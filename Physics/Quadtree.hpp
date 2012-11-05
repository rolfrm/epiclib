/*
 * Quadtree.hpp
 *
 *  Created on: Jul 4, 2012
 *      Author: sebastian
 */

#ifndef QUADTREE_HPP_
#define QUADTREE_HPP_

#define MAX_DEPTH 7

#include <vector>
#include "Math/Math.h"


class Polygon;

struct LineSegment;


class Quadnode{
public:
	Quadnode(std::vector<LineSegment *> & line_segments,Vec2 lower_bound,Vec2 upper_bound,int depth);
	Quadnode(Vec2 lower_bound,Vec2 upper_bound,int depth);
	~Quadnode();

	void LookUp(LineSegment * line_segment,std::vector<LineSegment *> & potential_collisions);
	void Insert(LineSegment * line_segment,std::vector<LineSegment *> & potential_collisions);
	void Insert(LineSegment * line_segment);

	void Draw(int level);
	void Clear();

	std::vector<LineSegment *> line_segments;
	Vec2 lower_bound,upper_bound;
	Quadnode * children[4];
	int depth;
	static long count;
private:
	bool IsLineContained(LineSegment * line_segment);
};


struct PolyQuadNode{
	unsigned int children[4];
	int depth;
	Vec2 lower_bound,upper_bound;
	std::vector<Polygon * > contained_polygons;
};

class PolyQuadTree{
public:
	PolyQuadTree(){};
	PolyQuadTree(Vec2 lower_bound,Vec2 upper_bound,int max_depth);
	~PolyQuadTree();

	void LookUp(Vec2 lower_bound,Vec2 upper_bound,std::vector<Polygon *> & potential_collisions,unsigned int node=0);
	void LookUp(Polygon * polygon,std::vector<Polygon *> & potential_collisions,unsigned int node=0);
	void Insert(Polygon * polygon,std::vector<Polygon *> & potential_collisions,unsigned int node=0);
	void Insert(Polygon * polygon,unsigned int node=0);

	void Draw(unsigned int node=0);

	void ClearTree();
	void ClearTree(Vec2 lower_bound,Vec2 upper_bound);

	friend class PolyQuadNode;
	int count;

private:
	int max_depth;
	unsigned int max_nodes,next_node;
	unsigned int make_node(Vec2 lower_bound,Vec2 upper_bound,int depth);
	PolyQuadNode * tree;

	bool isContained(Polygon * poly,unsigned int node);
	bool isContained(Vec2 lower_bound,Vec2 upper_bound,unsigned int node);
};


#endif /* QUADTREE_HPP_ */
