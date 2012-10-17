#pragma once
#include "../Graphics/VertexBufferObject.hpp"
#include <map>
/*Container class for multiple vbos
  Buffercollection?
*/
class Polygon{
  std::map<int, VertexBufferObject> bufferObjects;
  int size; 
public:
  Polygon();
  void Load(VertexBufferObject vbo, int _size, int bindPos);
  VertexBufferObject Get(int index);
  void Draw(DrawMethod drawMethod);
};
