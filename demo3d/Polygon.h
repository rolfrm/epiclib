#pragma once
#include "../Graphics/VertexBufferObject.hpp"
#include "../Graphics/Texture.hpp"
#include "../Utils/SharedPtr.h"
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
  void Load(VBO vbo, int bindPos);
  VertexBufferObject Get(int index);
  void Draw(DrawMethod drawMethod);
};

class Texgon{
  
  std::map<int, Texture> textures;
  public:
  void Load(Texture tex, int index);
  void Remove(int indexx);
  void Bind();
};


