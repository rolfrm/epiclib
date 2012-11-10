#pragma once
#include "AABB.h"
#include "../Math/Matrix.h"
#include "../Math/GenericVector.h"
#include "Tetragon.h"
#include "../Utils/SharedPtr.h"

class World;
class GameObject{
public:
  GameObject();
  Tetragon Tetra;
  AABBMass aabb;
  bool GravityBound;
  bool Movable;
  void DoUpdate(World & world);
  void Draw(Shader3D & shader);
  Vec<double,3> rotation;
 protected:
  virtual void Update(World & world);
};

typedef SharedPtr<GameObject *> GameObjectBase ;

