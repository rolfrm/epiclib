/*
 * VertexBufferObject.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: sebastian
 */

#include "VertexBufferObject.hpp"
#include <GL/glew.h>
#include <GL/gl.h>


VertexBufferObject::VertexBufferObject(float * data,unsigned int vertex_count,unsigned int vertex_dimension,unsigned int buffer_param){
	glGenBuffers(1,&reference);
	glBindBuffer(GL_ARRAY_BUFFER,reference);

	glBufferData(GL_ARRAY_BUFFER,vertex_count*sizeof(float),data,buffer_param);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	dataType = GL_FLOAT;

	count=new GLuint;
	*count=1;
	n_vertex=vertex_count;
	dim_vertex=vertex_dimension;
}


VertexBufferObject::VertexBufferObject(){
  count = NULL;
}

VertexBufferObject::VertexBufferObject(const void * data,GLuint data_size,GLenum buffer_param){
	glGenBuffers(1,&reference);
	glBindBuffer(GL_ARRAY_BUFFER,reference);

	glBufferData(GL_ARRAY_BUFFER,data_size,data,buffer_param);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	dataType = GL_FLOAT;
	count=new GLuint;
	*count=1;
	n_vertex=data_size;
	dim_vertex=0;
}

VertexBufferObject::VertexBufferObject(const VertexBufferObject & orginal){
	count=orginal.count;
	if(count != NULL){
	  *count +=1;
	}
	reference=orginal.reference;
	n_vertex=orginal.n_vertex;
	dim_vertex=orginal.dim_vertex;
	dataType = orginal.dataType;
}

VertexBufferObject::~VertexBufferObject(){
  if(count == NULL){
    return;
  }
  *count-=1;
	if(*count==0){
	
	  delete count;
		glDeleteBuffers(1,&reference);
	}
}

GLenum dataTypes[] = {GL_DOUBLE, GL_FLOAT, GL_INT};
GLenum vboDrawTypes[] = {GL_STATIC_DRAW,GL_DYNAMIC_DRAW,GL_STREAM_DRAW};
void VertexBufferObject::genVBO(void * data, int totalSize, int dim, int typeidx, VBODrawType drawtype){
  glGenBuffers(1,&reference);
  glBindBuffer(GL_ARRAY_BUFFER,reference);

  glBufferData(GL_ARRAY_BUFFER,totalSize,data,vboDrawTypes[(int) drawtype]);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  dataType = dataTypes[typeidx];

  count=new unsigned int;
  *count=1;
}
#include<iostream>
void VertexBufferObject::BindBuffer(GLuint index){
  glBindBuffer(GL_ARRAY_BUFFER,reference);
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index,dim_vertex,dataType,GL_FALSE,0,0);
}

GLenum drawMethods[] = {GL_POINTS, GL_TRIANGLES, GL_QUADS, GL_TRIANGLE_STRIP,GL_LINE_LOOP};
void VertexBufferObject::DrawBuffers(DrawMethod drawMethod,int nof){
  glDrawArrays(drawMethods[(int)drawMethod],0,nof);
}
