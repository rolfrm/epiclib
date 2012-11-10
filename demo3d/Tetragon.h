#pragma once
#include "Polygon.h"
#include "Shader.h"
#include "../Math/Matrix.h"

#include<list>
class TetragonBase{
public:
  TetragonBase(Polygon _pgon, Texgon _tgon);
  
  Polygon pgon;
  Texgon tgon;
  void Draw(Shader3D & shader);
};

class Tetragon: public TetragonBase{

 std::list<Tetragon> subGons;
  
 public:
 
 Tetragon(Matrix<float,4> tr, Polygon _pgon, Texgon _tgon);
 Tetragon();
 Matrix<float,4> TRS;
 void AttachTetragon(Tetragon tgon);
 void Draw(Shader3D &  shader,Matrix<float,4> T);
 Tetragon & SubGon(int idx);
};
