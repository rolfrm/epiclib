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
#include "../Utils/Debug.h"
bool AABBPhysics::CheckHandleCollision(AABBMass & aabb1, AABBMass & aabb2){

  Vec<double,3> d = aabb1.pos - aabb2.pos;
  Vec<double,3> aabb1c = aabb1.pos + aabb1.size;
  Vec<double,3> aabb2c = aabb2.pos + aabb2.size;
  Vec<double,3> overlap;
  Vec<double,3> sign;
  for(int i = 0; i < 3;i++){
    if(d[i] < 0){
      if(aabb2c[i] <= aabb1.pos[i]){
	return false;
      }
      else{
	overlap[i] = aabb2c[i] - aabb1.pos[i];
	sign[i] = 1.0;
      }
    }else{
      if(aabb1c[i] <= aabb2.pos[i]){
	return false;
      }else{
	overlap[i] = aabb1c[i] - aabb2.pos[i];
	sign[i] = -1.0;
      }
    }
  }
  double maxOverlap = 100000;
  int collisionAxis = -1;
  for(int i = 0; i<  3 ;i++){
    if(overlap[i] < maxOverlap){
      collisionAxis = i;
      maxOverlap = overlap[i];
    }
  }
  Vec<double,3> actualOverlap = vec(0.0,0.0,0.0);
  actualOverlap[collisionAxis] = overlap[collisionAxis] * sign[collisionAxis];
  double m1 = aabb1.mass.mass;
  double m2 = aabb2.mass.mass;
  double m1ratio = m2 /( m1 + m2);
  double m2ratio = -m1 / (m1 + m2);
 
  double cv1 = aabb1.mass.vel[collisionAxis];
  double cv2 = aabb2.mass.vel[collisionAxis];
  
  aabb1.pos = aabb1.pos + actualOverlap * m1ratio;
  aabb2.pos = aabb2.pos + actualOverlap * m2ratio;
  
  double dv = cv1 - cv2;

  double j = dv / ( 1.0 / m1 + 1.0 / m2);
  aabb1.mass.vel[collisionAxis] += -j / m1;
  aabb2.mass.vel[collisionAxis] += j / m2;
  return true;
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


PhysicsIterationResult AABBListPhysics::PhysicsIteration2(std::list<AABBMass> aabbs, double dt){
  std::list<AABBMass> outList;
  for(AABBMass aabb : aabbs){
    outList.push_back(UpdatePosition(aabb,0.01));
  }
  PhysicsIterationResult pir;
  for(auto a = outList.begin(); a != outList.end();a++){
    auto b = a;
    b++;
    for(;b != outList.end();b++){
      bool collided = CheckHandleCollision(*a,*b);
      if(collided){
	
      }
    }
  }


}
