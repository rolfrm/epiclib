#include "World.h"
#include "../Utils/Debug.h"
typedef std::list<GameObjectBase> goList;
goList World::GetNearbyObjects(Vec<double,3> pos, double radius){
  goList out;
  for(auto it = gameObjects.begin(); it != gameObjects.end();it++){
    AABB aabb = (*it).Get()->aabb;
    Vec<double,3> distanceVector = aabb.pos - pos;
    double distance = VectorLength(distanceVector);
    if(distance < radius){
      out.push_back(*it);
    }
    
  }
  return out;

}

void World::InsertObject(GameObjectBase gameObject){
  gameObjects.push_back(gameObject);
}

void World::RemoveObject(GameObjectBase gameObject){
  gameObjects.remove(gameObject);
}

GameObjectBase World::Find(GameObject & gameObject){

}

void World::PhysicsUpdate(Vec<double,3> pos, double radius, double dt){
  std::list<GameObjectBase> golist = GetNearbyObjects(pos,radius);
  
  double gravity = 0.1;//9.8;

  std::list<AABBMass> masses;
  for(GameObjectBase gob : golist){
    if(gob.Get()->GravityBound){
      gob.Get()->aabb.mass.ApplyImpulse(vec(0.0,-gravity,0.0));
    }
    masses.push_back(gob.Get()->aabb);
  }
  
  AABBListPhysics physicsHandler;
  PhysicsIterationResult result = physicsHandler.PhysicsIteration2(masses,dt);
  masses = result.NewMasses;
  for(AABBMass aabb : masses){
    (*(golist.begin())).Get()->aabb = aabb;
    golist.pop_front();
  }
  
}
