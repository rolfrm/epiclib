#version 130
#extension GL_EXT_gpu_shader4 : enable
/*#pragma optionNV(fastmath on)
#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)*/
//#pragma optionNV(unroll all)

varying vec3 localPos;
varying vec3 direction;
uniform sampler2D map;
uniform int width;
uniform int height;
uniform vec3 camPos;
int colorToInt(vec4 color){
    return int(color.r * 255.0f) | (int(color.g * 255.0f) << 8) | (int(color.b * 255.0f) << 16) | (int(color.a * 255.0f) << 24);
}

uint colorToUInt(vec4 color){
    return uint(color.r * 255.0f) | (uint(color.g * 255.0f) << uint(8)) | (uint(color.b * 255.0f) << uint(16)) | (uint(color.a * 255.0f) << uint(24));
}


vec4 intToColor(int i){
     float ri = float(i & 0xFF);
     float gi = float((i >> 8 ) & 0xFF);
     float bi = float((i >> 16) & 0xFF);
     float ai = float((i >> 24) & 0xFF);
     return vec4(ri,gi,bi,ai) / 255.0f;
}
vec4 uintToColor(uint i){
     int ii = int(i);
     float ri = float(ii & 0xFF);
     float gi = float((ii >> 8 )  & 0xFF);
     float bi = float((ii >> 16) & 0xFF);
     float ai = float((i >> uint(24)) % uint(256));
     return vec4(ri,gi,bi,ai) / 255.0f;

}

ivec2 idxToIvec(int idx){
     return ivec2(idx % width,idx / width); 
}

vec4 getData(ivec2 p){
     return texelFetch(map,p,0);
}
vec4 getDataIdx(int idx){
     return getData(idxToIvec(idx));     
}

int idxGetData(int cell, int child_indx){
    return colorToInt( getDataIdx(cell + child_indx + 3));
}

uint idxGetDatau(int cell,int child_indx){
    return colorToUInt( getDataIdx(cell + child_indx + 3));
}

bool idxIsChild(uint cellD, int child_idx){
     int v = int(cellD);// colorToInt(getDataIdx(cell));
     return ( ((v >> child_idx) & 1) == 1);
}

bool idxHasChildData(uint cellD, int child_idx){
     int v = int(cellD);
     return ( ((v >> (child_idx + 16)) & 1) == 1);
}


vec4 idxGetRGBData(int cell, int child_indx){
     return texelFetch(map,idxToIvec(cell + child_indx + 3),0);
}




int getParent(int cellindex){
    uint parentU =  colorToUInt(getDataIdx(cellindex + 1));
    return int(parentU);
}

vec3 getRelPos(uint metaData){
     uint index = metaData >> uint(9);
     
     return vec3(index & uint(1),float((index >> uint(1)) & uint(1)) ,float((index >> uint(2)) & uint(1)));
}

float calc_rec_len(float d, float p){

      float o = floor(p);
      if(d >= 0){
	o += 1.0001;
      }else{
	o -= 0.0001;
      }
      return (o - p ) / d;
}


#define MAX_IT 20
void main(){
    
    vec3 dir = normalize(vec3(direction.xy,-direction.z));
    vec3 dirv;
    dirv.x = dir.x >= 0 ? 1.0005 : -0.0005; 
    dirv.y = dir.y >= 0 ? 1.0005 : -0.0005;
    dirv.z = dir.z >= 0 ? 1.0005 : -0.0005;
    vec3 p = localPos + dir*0.0001;
   
    int currentCell = 0;
    int i;
    
    float lod = -1;
    float lodTreshold = -100;
    float d = 0;
    vec3 maxLimit = vec3(2);
    vec3 minLimit = vec3(0);
    for(i = 0; i < MAX_IT;i++){
      uint currentCellData = colorToUInt(getDataIdx(currentCell));
      
      if(any(greaterThanEqual(p,maxLimit)) || any(lessThan(p,minLimit))){
      	currentCell = currentCell + getParent(currentCell);
	if(currentCell == 0){
          break;
	}
	lod += 1;
	p = p / 2.0f + getRelPos(currentCellData);
	continue;
      }
      vec3 cell = floor(p);
      float index = dot(cell, vec3(1,2,4));
      bool isSubTree = idxIsChild(currentCellData, int(index));
      bool hasChildData = idxHasChildData(currentCellData,int(index));
      
      int lookupIdx = int(index);
      
      if(lodTreshold >= lod){
         lookupIdx = -1;
      }
      vec4 rgbData = getDataIdx(currentCell + lookupIdx +3);
      uint data = colorToUInt(rgbData);
      if(data > uint(0) && (hasChildData || lodTreshold >= lod) ){
	gl_FragColor = rgbData; 
	break;
	
      }else if(isSubTree && (data != uint(0) && lodTreshold < lod)){	
	lod -= 1;
        currentCell += int(data);
	p = 2 * (p - cell);
	continue; 
      }
      vec3 dp = (dirv - fract(p) ) / dir;
      float dt = dp.z;
      if(dp.y < dp.z){
	if(dp.x < dp.y){
	 dt = dp.x;
	}else{
         dt = dp.y;
	}
      }

      d += pow(2,lod)*dt;
      p += dir * dt;
      lodTreshold =-9;
    }
    //gl_FragColor = vec4(vec3(int(i > 40)),1);
}    
