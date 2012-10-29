/*
 * VertexBufferObject.hpp
 *
 *  Created on: Aug 4, 2012
 *      Author: sebastian
 */

#ifndef VERTEXBUFFEROBJECT_HPP_
#define VERTEXBUFFEROBJECT_HPP_
#include "oglCppWrap.h"
#include "../Utils/SharedPtr.h"
enum class VBODrawType{Static = 0,Dynamic = 1,Stream = 2};
enum class DrawMethod{Points = 0, Triangles = 1, Quads = 2, TriangleStrip = 3, LineLoop=4};

class _bufferObject{
public:
  unsigned int gl_ref;
  
  _bufferObject(){
    gl_ref = 999999;
  }
  _bufferObject(int _gl_ref){
    gl_ref = _gl_ref;
  }
  void Dispose();
};

class VertexBufferObject{
  SharedPtr<_bufferObject> bufferObject;
  void genVBO(void * data, int totalSize, int dim, int typeidx, VBODrawType drawtype);

public:


  template<class T>
  VertexBufferObject(T * data, unsigned int vertex_count,int dim, VBODrawType drawType){
    T null;
    int typeIdx = getTypeIndex(null);
    n_vertex = vertex_count;
    dim_vertex = dim;
    genVBO((void *) data,vertex_count*dim*sizeof(T),dim,typeIdx,drawType);
  }

  VertexBufferObject();

  void BindBuffer(unsigned int index);
  unsigned int n_vertex,dim_vertex, dataType;

  static void DrawBuffers(DrawMethod drawMethod,int nof);
 
};





#endif /* VERTEXBUFFEROBJECT_HPP_ */
