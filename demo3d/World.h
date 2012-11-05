#include <list>
#include "GameObject.h"
#include "../Math/GenericVector.h"
class World{
  std::list<GameObjectBase> gameObjects;
 public:
  std::list<GameObjectBase> GetNearbyObjects(Vec<double,3> pos, double radius);
  void InsertObject(GameObjectBase gameObject);
  void RemoveObject(GameObjectBase gameObject);

  GameObjectBase Find(GameObject & gameObject);
  void PhysicsUpdate(Vec<double,3> pos, double radius, double dt);

};
