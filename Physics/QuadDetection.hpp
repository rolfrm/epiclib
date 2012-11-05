/*
 * QuadDetection.hpp
 *
 *  Created on: Jul 19, 2012
 *      Author: sebastian
 */

#ifndef QUADDETECTION_HPP_
#define QUADDETECTION_HPP_

#include "Math/Math.h"
#include <vector>
#include <list>


class Polygon;
class PolyQuadTree;
class LineSegment;
class PhysicsObject;

struct collision_node{
	PhysicsObject * from, * to;
	Vec2 normal,tangent,contact,arm_from,arm_to;
	double overlap;
	unsigned int from_edge,to_edge;
};


class QuadtreeCollisionDetecter{
public:
	QuadtreeCollisionDetecter();
	~QuadtreeCollisionDetecter();

	void GenerateTree();

	void CollisionDetection(Polygon * input,bool insert=false);

	PolyQuadTree * quad_tree;
	std::vector<collision_node> output;
	std::list<PhysicsObject *> active_objects;
	int counter;
private:
	collision_node LineCollisionDetection(LineSegment * line1,LineSegment * line2,collision_node & return_node);
	void SAT(Vec2 Axis1,Vec2 Axis2,Polygon * poly1, Polygon * poly2,Vec2 & Axis,double & overlap,PhysicsObject ** signal);
};




#endif /* QUADDETECTION_HPP_ */
