/*
 * VertexBufferObject.hpp
 *
 *  Created on: Aug 4, 2012
 *      Author: sebastian
 */

#ifndef VERTEXBUFFEROBJECT_HPP_
#define VERTEXBUFFEROBJECT_HPP_
#include "oglCppWrap.h"
enum class VBODrawType{STATIC = 0,DYNAMIC = 1,STREAM = 2};
enum class DrawMethod{POINTS = 0, TRIANGLES = 1, QUADS = 2, TRIANGLE_STRIP = 3, LINE_LOOP=4};

class VertexBufferObject{

  void genVBO(void * data, int totalSize, int dim, int typeidx, VBODrawType drawtype);

public:
  VertexBufferObject(float * data,unsigned int vertex_count, unsigned int vertex_dimension,unsigned int drawType);
  VertexBufferObject(const void * data,unsigned int data_size,unsigned int drawType);

  template<class T>
  VertexBufferObject(T * data, unsigned int vertex_count,int dim, VBODrawType drawType){
    T null;
    int typeIdx = getTypeIndex(null);
    n_vertex = vertex_count;
    dim_vertex = dim;
    genVBO((void *) data,vertex_count*dim*sizeof(T),dim,typeIdx,drawType);
  }

  VertexBufferObject(const VertexBufferObject & orginal);
  ~VertexBufferObject();

  void BindBuffer(unsigned int index=0);
  unsigned int * count;
  unsigned int reference,n_vertex,dim_vertex, dataType;

  static void DrawBuffers(DrawMethod drawMethod,int nof);
  static void DrawBuffers(DrawMethod drawMethod,int nof,VertexBufferObject &v1){
    v1.BindBuffer(0);
    DrawBuffers(drawMethod,nof);
  }
  static void DrawBuffers(DrawMethod drawMethod,int nof,VertexBufferObject &v1, VertexBufferObject &v2){
    v2.BindBuffer(1);
    DrawBuffers(drawMethod,nof,v1);
  }
  static void DrawBuffers(DrawMethod drawMethod,int nof,VertexBufferObject &v1, VertexBufferObject &v2, VertexBufferObject &v3){
    v3.BindBuffer(2);
    DrawBuffers(drawMethod,nof,v1,v2);
  }
  static void DrawBuffers(DrawMethod drawMethod,int nof,VertexBufferObject &v1, VertexBufferObject &v2, VertexBufferObject &v3,VertexBufferObject &v4){
    v4.BindBuffer(3);
    DrawBuffers(drawMethod,nof,v1,v2,v3);
  }

};





#endif /* VERTEXBUFFEROBJECT_HPP_ */
