
#pragma once
#include "../Math/GenericVector.h"
#include <list>
class AABB{
public:
  Vec<double,3> pos;
  Vec<double,3> size;
  AABB(Vec<double,3> _pos, Vec<double,3> _size);
};

class Newtonian{
 public:
  Vec<double,3> vel;
  Vec<double,3> acc;
  double mass;
  void ApplyImpulse(Vec<double,3> ForceVector);
  Newtonian(double mass);

};

class AABBMass: public AABB{
 public:
  Newtonian mass;
  AABBMass(Vec<double,3> pos, Vec<double,3> size, double _mass);
};

class AABBPhysics{
 public:
  struct CollisionPair{
    AABBMass aabb1;
    AABBMass aabb2;
  };
  AABBMass UpdatePosition(AABBMass abMass,double t);
  Vec<double,3> calculateOverlap(AABB aabb1, AABB aabb2);
  CollisionPair CalculateCollision(CollisionPair pair);  
};

class AABBListPhysics: public AABBPhysics{
 public:
  std::list<AABBMass> PhysicsIteration(std::list<AABBMass> aabbs);
};
