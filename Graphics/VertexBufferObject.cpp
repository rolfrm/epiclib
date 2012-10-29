/*
 * VertexBufferObject.cpp
 *
 *  Created on: Aug 4, 2012
 *      Author: sebastian
 */

#include "VertexBufferObject.hpp"
#include <GL/glew.h>
#include <GL/gl.h>


VertexBufferObject::VertexBufferObject(){

}

GLenum dataTypes[] = {GL_DOUBLE, GL_FLOAT, GL_INT};
GLenum vboDrawTypes[] = {GL_STATIC_DRAW,GL_DYNAMIC_DRAW,GL_STREAM_DRAW};
void VertexBufferObject::genVBO(void * data, int totalSize, int dim, int typeidx, VBODrawType drawtype){
 
  glGenBuffers(1,&(bufferObject.Get().gl_ref));
  glBindBuffer(GL_ARRAY_BUFFER,bufferObject.Get().gl_ref);

  glBufferData(GL_ARRAY_BUFFER,totalSize,data,vboDrawTypes[(int) drawtype]);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  dataType = dataTypes[typeidx];

}

void VertexBufferObject::BindBuffer(GLuint index){
  glBindBuffer(GL_ARRAY_BUFFER,bufferObject.Get().gl_ref);
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index,dim_vertex,dataType,GL_FALSE,0,0);
}

GLenum drawMethods[] = {GL_POINTS, GL_TRIANGLES, GL_QUADS, GL_TRIANGLE_STRIP,GL_LINE_LOOP};

void VertexBufferObject::DrawBuffers(DrawMethod drawMethod,int nof){
  glDrawArrays(drawMethods[(int)drawMethod],0,nof);
}
#include <iostream>
void _bufferObject::Dispose(){
  if(gl_ref != 999999){
    
    std::cout << "Disposing VBO " << gl_ref << "\n";
  
    glDeleteBuffers(1,&gl_ref);
  }
}
