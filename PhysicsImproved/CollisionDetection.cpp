#include <math.h>

#include "CollisionDetection.hpp"
#include "Polygon.hpp"


bool CollisionDetector::polygonDetection(Polygon & p1,Polygon & p2){
  
  
}

bool CollisionDetector::broadPhaseTriangleDetection(Triangle& t1, Triangle& t2)
{
  if((t1.Center-t2.Center).getNorm2Squared()<pow(t1.radius+t2.radius,2))
    return true;
  
  return false;
}

bool CollisionDetector::narrowPhaseTriangleDetection(Triangle& t1, Triangle& t2)
{
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      if(LineIntersection(t1.Vertex[i],t1.Edge[i],t2.Vertex[j],t2.Edge[j])){
	return true;
      }
    }
  }
  
  int contained_in=0;
  
  for(int i=0;i<3;i++){
    if(t2.pointInside(t1.Vertex[i]))
      contained_in++;
  }
  
  if(contained_in==3)
    return true;
  
  contained_in=0;
  
  for(int i=0;i<3;i++){
    if(t1.pointInside(t2.Vertex[i]))
      contained_in++;
  }
  
  if(contained_in==3)
    return true;
  
  return false;
}
void CollisionDetector::axisOfMinimalOverlap(Polygon * p1,Triangle & t1,Polygon * p2,Triangle & t2,unsigned int index1,unsigned int index2,collision_node & return_node)
{
    double min=INFINITY,max=t1.Normal[index1]*t1.Vertex[index1];
    
    for(int i=0;i<3;i++){
      double temp = t1.Normal[index1]*t2.Vertex[i];
      if(temp<min)
	min=temp;  
    }
    
    return_node.overlap=max-min;
    
    return_node.from=p1,return_node.to=p2;
    
    min=INFINITY;
    max=t2.Normal[index2]*t2.Vertex[index2];
       
    for(int i=0;i<3;i++){
      double temp = t2.Normal[index2]*t1.Vertex[i];
      if(temp<min)
	min=temp;  
    }
    
    if(max-min<return_node.overlap){
      return_node.overlap=max-min;
      return_node.from=p2,return_node.to=p1;
    }
    
    
}

