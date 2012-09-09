/*
 * VertexBufferObject.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: sebastian
 */

#include "VertexBufferObject.hpp"


VertexBufferObject::VertexBufferObject(GLfloat * data,GLuint vertex_count,GLuint vertex_dimension,GLenum buffer_param){
	glGenBuffers(1,&reference);
	glBindBuffer(GL_ARRAY_BUFFER,reference);

	glBufferData(GL_ARRAY_BUFFER,vertex_count*sizeof(GLfloat),data,buffer_param);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	count=new GLuint;
	*count=1;
	n_vertex=vertex_count;
	dim_vertex=vertex_dimension;
}

VertexBufferObject::VertexBufferObject(const VertexBufferObject & orginal){
	count=orginal.count;
	*count+=1;
	reference=orginal.reference;
	n_vertex=orginal.n_vertex;
	dim_vertex=orginal.dim_vertex;
}

VertexBufferObject::~VertexBufferObject(){
	*count-=1;
	if(*count==0){
		delete count;
		glDeleteBuffers(1,&reference);
	}
}


void VertexBufferObject::BindBuffer(GLuint index){
	glBindBuffer(GL_ARRAY_BUFFER,reference);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index,dim_vertex,GL_FLOAT,GL_FALSE,0,0);


}



