#pragma once
#include "../Graphics/VertexBufferObject.hpp"
#include "../Graphics/Texture.hpp"
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

class Texgon{
  
  std::map<int, Texture2D> textures;
  public:
  void Load(Texture2D tex, int index);
  void Remove(int index);
  void Bind();
};


