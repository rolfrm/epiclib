#include <vector>
#include <map>

#include "../Math/Math.hpp"

class Polygon;
class Triangle;

struct collision_node{
  Polygon * from,* to;
  Vec2 seperating_axis,contact;
  double overlap;
};


class CollisionDetector{
public:
  CollisionDetector();
  
  bool polygonDetection(Polygon & p1,Polygon & p2);
  
  bool narrowPhaseTriangleDetection(Triangle & t1,Triangle & t2);
  
  bool broadPhaseTriangleDetection(Triangle & t1,Triangle & t2);
  
  void axisOfMinimalOverlap(Polygon * p1,Triangle & t1,Polygon * p2,Triangle & t2,unsigned int index1,unsigned int index2,collision_node & return_node);
  
};
