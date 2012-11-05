#include "AABB.h"
#include "../Utils/Debug.h"
AABB::AABB(Vec<double,3> _pos, Vec<double,3> _size){
  pos = _pos;
  size = _size;
}

Newtonian::Newtonian(double _mass){
  mass = _mass;
  vel = vec(0.0,0.0,0.0);
  acc = vec(0.0,0.0,0.0);
}

void Newtonian::ApplyImpulse(Vec<double,3> ImpulseVector){
  vel = vel + ImpulseVector / mass;
}
AABBMass::AABBMass(Vec<double,3> pos, Vec<double,3> size, double _mass): AABB(pos,size),mass(_mass){
  

}


Vec<double,3> AABBPhysics::calculateOverlap(AABB aabb1, AABB aabb2){
  
}

AABBMass AABBPhysics::UpdatePosition( AABBMass aabb, double t){
  aabb.mass.vel = aabb.mass.vel + aabb.mass.acc * t;
  aabb.pos = aabb.pos + aabb.mass.vel * t;
  return aabb;
}

AABBPhysics::CollisionPair AABBPhysics::CalculateCollision(AABBPhysics::CollisionPair pair){
  
}

std::list<AABBMass> AABBListPhysics::PhysicsIteration(std::list<AABBMass> aabbs)
{
  std::list<AABBMass> outList;
  for(AABBMass aabb : aabbs){
    outList.push_back(UpdatePosition(aabb,0.01));
  }
  /*std::list<AABBMass> rest = outList;
  rest.pop_front();
  for(AABBMass & aabb: outList){
    for(AABBMass & aabb2: outList){
      
    }
    }*/
  return outList;
}
