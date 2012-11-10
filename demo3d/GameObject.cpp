#include "GameObject.h"
#include "../Math/CommonMatrixOperations.h"
#include "World.h"
#include "../Utils/Debug.h"
GameObject::GameObject():
  Tetra(Matrix<float,4>::Eye(),Polygon(),Texgon()), 
  aabb(vec(0.0,0.0,0.0),vec(1.0,1.0,1.0),1.0){
  GravityBound = true;
  rotation = vec(0.0,0.0,0.0);
}

void GameObject::DoUpdate(World & world){
  Update(world);
}

void GameObject::Update(World & world){
  
}

void GameObject::Draw(Shader3D & shader){
  
  auto t = TMatrix(aabb.pos.As<float>()) * RMatrix(rotation.As<float>());
  
  Tetra.Draw(shader,t);
}
