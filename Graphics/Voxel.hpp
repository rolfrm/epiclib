#include <vector>
#include <GL/gl.h>
#include <iostream>

#include "VertexBufferObject.hpp"
#include "Program.hpp"
#include "../Math/CommonMatrixOperations.h"

class VoxelRender{
public:
	VoxelRender(unsigned int dim_x,unsigned int dim_y,unsigned int dim_z);
	VoxelRender(unsigned int dim_x,unsigned int dim_y,unsigned int dim_z,float voxel_size);
	VoxelRender(const VoxelRender & original);
	~VoxelRender();
	
	
	void set(unsigned int x,unsigned int y,unsigned int z,bool value){
		if(x<dim[0] && y<dim[1] && z<dim[2])
			voxel_grid[x+y*dim[0]+z*dim[1]*dim[0]]=value;
	}
	
	bool get(unsigned int x,unsigned int y,unsigned z){
		if(x<dim[0] && y<dim[1] && z<dim[2]){
			return voxel_grid[x+y*dim[0]+z*dim[1]*dim[0]];	
		}
	}
	
	void clear(){
		for(unsigned int i=0;i<dim[0]*dim[1]*dim[2];i++){
			voxel_grid[i]=false;
		}
	
	}
	
	void makeVoxelMesh();
	
	virtual void draw(Matrix<float,4> & world2view,Matrix<float,4> & view2perspective);
	
	bool * voxel_grid;
	
	Vec<float,3> position,angle,scale;
	
	unsigned int dim[3];
	
protected:
	
	VertexBufferObject voxel_grid_vbo;
	
	Program voxel_draw;
	
	unsigned int * count;
	
	std::vector<unsigned int> upper_draw,lower_draw,right_draw,left_draw,front_draw,back_draw;
	
	void insertVoxel(unsigned int x,unsigned int y,unsigned int z);
	
	inline unsigned int makeIndex(unsigned int x,unsigned int y,unsigned int z){
		return (x+y*(dim[0]+1)+z*(dim[0]+1)*(dim[1]+1));
	}
	
	Matrix<float,4> object2world;

};
