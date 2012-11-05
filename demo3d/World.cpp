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
  std::list<AABBMass> masses;
  for(GameObjectBase gob : golist){
    masses.push_back(gob.Get()->aabb);
  }
  
  AABBListPhysics physicsHandler;
  masses = physicsHandler.PhysicsIteration(masses);
  
  for(AABBMass aabb : masses){
    (*(golist.begin())).Get()->aabb = aabb;
    golist.pop_front();
  }
  
}
