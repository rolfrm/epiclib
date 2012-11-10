#include "Tetragon.h"
#include "../Utils/Debug.h"
TetragonBase::TetragonBase(Polygon _pgon, Texgon _tgon){
  pgon = _pgon;
  tgon = _tgon;
}

void TetragonBase::Draw(Shader3D & shader){
  tgon.Bind();
  pgon.Draw(shader.GetDrawMethod());
  
}

Tetragon::Tetragon(Matrix<float,4> tr, Polygon _pgon, Texgon _tgon):TetragonBase(_pgon,_tgon){
  TRS = tr;
  
}
Tetragon::Tetragon():TetragonBase(Polygon(),Texgon()){
  
}

void Tetragon::AttachTetragon(Tetragon tgon){
  subGons.push_back(tgon);
}

void Tetragon::Draw(Shader3D & shader,Matrix<float,4> T){
  Matrix<float,4> relative = T * TRS;
  shader.SetModelView(relative);
  TetragonBase::Draw(shader);
  for(auto it = subGons.begin(); it != subGons.end();it++){
    (*it).Draw(shader,relative);
  }
}

Tetragon & Tetragon::SubGon(int idx){
  
  if(subGons.size() <= idx){
    throw "Error, no such subGon";
  }
  int idx_it= 0;
  auto it = subGons.begin();
  for(int i = 0; i < idx_it;i++){
    it++;
  }
  return *it;
}
