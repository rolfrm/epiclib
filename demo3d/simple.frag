uniform sampler2D tex;
uniform vec4 Color;
varying vec3 pos;
void main(){
     
     gl_FragColor = Color * (sin(pos.x)*0.2 + 0.8)*(cos(pos.z)*0.2 + 0.8)* (cos(pos.x*0.1)*0.2 + 0.8)*(sin(pos.z*0.1)*0.2 + 0.8)* (cos(pos.x*0.01)*0.2 + 0.8)*(sin(pos.z*0.01)*0.2 + 0.9)* texture2D(tex,pos.xz);
     //gl_FragColor = texture2D(tex,pos.xz);
}    
