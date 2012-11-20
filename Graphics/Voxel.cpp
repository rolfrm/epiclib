#include "Voxel.hpp"


VoxelRender::VoxelRender(unsigned int dim_x,unsigned int dim_y,unsigned int dim_z){
	dim[0]=dim_x;
	dim[1]=dim_y;
	dim[2]=dim_z;
	
	voxel_grid=new bool[dim[0]*dim[1]*dim[2]];
	
	clear();
	
	float * voxel_vertex = new float[(dim[0]+1)*(dim[1]+1)*(dim[2]+1)*4];
	
	
	for(unsigned int i=0;i<dim[0]+1;i++){
		for(unsigned int j=0;j<dim[1]+1;j++){
			for(unsigned int k=0;k<dim[2]+1;k++){
				unsigned int index=(i+j*(dim[0]+1)+k*(dim[0]+1)*(dim[1]+1))*4;
				
				voxel_vertex[index]=(float)i*2.0f/(float)dim[0]-1.0f;
				voxel_vertex[index+1]=(float)j*2.0f/(float)dim[1]-1.0f;
				voxel_vertex[index+2]=(float)k*2.0f/(float)dim[2]-1.0f;
				voxel_vertex[index+3]=1.0f;
			}
		}
	}
	
	
	
	voxel_grid_vbo = VertexBufferObject(voxel_vertex,(dim[0]+1)*(dim[1]+1)*(dim[2]+1),4,VBODrawType::Static);
	
	delete [] voxel_vertex;
	
	voxel_draw = Program("Shader/voxel.vert","Shader/voxel.frag"); //Should probably be subject to change, like hardcoding the shader in or something
	
	scale=vec(1.0f,1.0f,1.0f);
	
	count = new unsigned int;
	*count=1;
  
}



VoxelRender::VoxelRender(unsigned int dim_x,unsigned int dim_y,unsigned int dim_z,float voxel_size){
	dim[0]=dim_x;
	dim[1]=dim_y;
	dim[2]=dim_z;
	
	voxel_grid=new bool[dim[0]*dim[1]*dim[2]];
	
	clear();
	
	float * voxel_vertex = new float[(dim[0]+1)*(dim[1]+1)*(dim[2]+1)*4];
	
	
	for(unsigned int i=0;i<dim[0]+1;i++){
		for(unsigned int j=0;j<dim[1]+1;j++){
			for(unsigned int k=0;k<dim[2]+1;k++){
				unsigned int index=(i+j*(dim[0]+1)+k*(dim[0]+1)*(dim[1]+1))*4;
				
				voxel_vertex[index]=(float)i*2.0f*voxel_size-1.0f/voxel_size;
				voxel_vertex[index+1]=(float)j*2.0f*voxel_size-1.0f/voxel_size;
				voxel_vertex[index+2]=(float)k*2.0f*voxel_size-1.0f/voxel_size;
				voxel_vertex[index+3]=1.0f;
				
				if(index==0){
					std::cout<<(float)i*2.0f*voxel_size-1.0f/voxel_size<<" "<<voxel_vertex[index]<<" "<<voxel_vertex[index+1]<<" "<<voxel_vertex[index+2]<<" "<<voxel_vertex[index+3]<<" iceeee\n";
				}
			}
		}
	}
	
	
	
	voxel_grid_vbo = VertexBufferObject(voxel_vertex,(dim[0]+1)*(dim[1]+1)*(dim[2]+1),4,VBODrawType::Static);
	
	delete [] voxel_vertex;
	
	voxel_draw = Program("Shader/voxel.vert","Shader/voxel.frag"); //Should probably be subject to change, like hardcoding the shader in or something
	
	scale=vec(1.0f,1.0f,1.0f);
	
	count = new unsigned int;
	*count=1;
  
}

VoxelRender::~VoxelRender(){
	*count-=1;
	
	if(count==0){
		delete count;
		delete [] voxel_grid;
	
	}

}


void VoxelRender::insertVoxel(unsigned int x,unsigned int y,unsigned int z){
	
	unsigned int low_array[] = {makeIndex(x,y,z),makeIndex(x+1,y,z+1),makeIndex(x,y,z+1),makeIndex(x,y,z),makeIndex(x+1,y,z),makeIndex(x+1,y,z+1)};
	lower_draw.insert(lower_draw.end(),low_array,low_array+6);
	
	unsigned int up_array[]= {makeIndex(x,y+1,z),makeIndex(x,y+1,z+1),makeIndex(x+1,y+1,z+1),makeIndex(x,y+1,z),makeIndex(x+1,y+1,z+1),makeIndex(x+1,y+1,z)};
	upper_draw.insert(upper_draw.end(),up_array,up_array+6);
	
	unsigned int right_array[]={makeIndex(x+1,y,z),makeIndex(x+1,y+1,z),makeIndex(x+1,y,z+1),makeIndex(x+1,y+1,z),makeIndex(x+1,y+1,z+1),makeIndex(x+1,y,z+1)};
	right_draw.insert(right_draw.end(),right_array,right_array+6);
	
	unsigned int left_array[]={makeIndex(x,y,z),makeIndex(x,y,z+1),makeIndex(x,y+1,z),makeIndex(x,y+1,z),makeIndex(x,y,z+1),makeIndex(x,y+1,z+1)};
	left_draw.insert(left_draw.end(),left_array,left_array+6);
	
	unsigned int front_array[]={makeIndex(x,y,z),makeIndex(x,y+1,z),makeIndex(x+1,y+1,z),makeIndex(x,y,z),makeIndex(x+1,y+1,z),makeIndex(x+1,y,z)};
	front_draw.insert(front_draw.end(),front_array,front_array+6);
	
	unsigned int back_array[]={makeIndex(x,y,z+1),makeIndex(x+1,y+1,z+1),makeIndex(x,y+1,z+1),makeIndex(x,y,z+1),makeIndex(x+1,y,z+1),makeIndex(x+1,y+1,z+1)};
	back_draw.insert(back_draw.end(),back_array,back_array+6);

}

void VoxelRender::makeVoxelMesh(){
	upper_draw.clear();
	lower_draw.clear();
	right_draw.clear();
	left_draw.clear();
	front_draw.clear();
	back_draw.clear();
	
	
	for(int x=0;x<dim[0];x++){
		for(int y=0;y<dim[1];y++){
			for(int z=0;z<dim[2];z++){
				if(get(x,y,z)){
					if(x==0 || y==0 || z==0 || x==dim[0]-1 || y==dim[1]-1 || z==dim[2]-1){
						insertVoxel(x,y,z);
					}
					else if(!get(x,y,z+1) || !get(x,y,z-1) || !get(x,y+1,z) || !get(x,y-1,z) || !get(x+1,y,z) || !get(x-1,y,z)){
						insertVoxel(x,y,z);
					}
				}
			}
		}
	}
	
}

void VoxelRender::draw(Matrix<float,4> & world2view,Matrix<float,4> & view2perspective){
	object2world=TRSMatrix(position,angle,scale);
	
	voxel_draw.UseProgram();
	
	voxel_draw.setUniformMat4x4("object2world",object2world.AsPtr());
	voxel_draw.setUniformMat4x4("world2view",world2view.AsPtr());
	voxel_draw.setUniformMat4x4("view2perspective",view2perspective.AsPtr());
	
	 	
  	voxel_draw.setUniform("normal",0.0f,1.0f,0.0f);
		  	 	  	
  	glDrawElements(GL_TRIANGLES,upper_draw.size(),GL_UNSIGNED_INT,upper_draw.data());
		  	
   	voxel_draw.setUniform("normal",0.0f,-1.0f,0.0f);
	  	
	glDrawElements(GL_TRIANGLES,lower_draw.size(),GL_UNSIGNED_INT,lower_draw.data());
		
	voxel_draw.setUniform("normal",1.0f,0.0f,0.0f);
		
	glDrawElements(GL_TRIANGLES,right_draw.size(),GL_UNSIGNED_INT,right_draw.data());
		
	voxel_draw.setUniform("normal",-1.0f,0.0f,0.0f);
		
	glDrawElements(GL_TRIANGLES,left_draw.size(),GL_UNSIGNED_INT,left_draw.data());
		
	voxel_draw.setUniform("normal",0.0f,0.0f,-1.0f);
		
	glDrawElements(GL_TRIANGLES,front_draw.size(),GL_UNSIGNED_INT,front_draw.data());
		
	voxel_draw.setUniform("normal",0.0f,0.0f,1.0f);
		
	glDrawElements(GL_TRIANGLES,back_draw.size(),GL_UNSIGNED_INT,back_draw.data());
	
}
