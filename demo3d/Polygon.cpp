#include "Polygon.h"
Polygon::Polygon(){
    size = -1;
  }

void Polygon::Load(VertexBufferObject vbo, int _size, int bindPos){
  if(size != -1 && size != _size){
    return;
  }
  size = _size;
  bufferObjects[bindPos] = vbo;
}
  
VertexBufferObject Polygon::Get(int index){
  return bufferObjects[index];
}

void Polygon::Draw(DrawMethod drawMethod){
  if(size == -1){
    return;
  }
  for(auto it = bufferObjects.begin(); it != bufferObjects.end();it++){
    VertexBufferObject & vbo = it->second;
    int bindloc = it->first;
    vbo.BindBuffer(bindloc);
  }
  VertexBufferObject::DrawBuffers(drawMethod,size);
  
}
