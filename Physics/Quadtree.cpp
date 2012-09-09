/*
 * Quadtree.cpp
 *
 *  Created on: Jul 4, 2012
 *      Author: sebastian
 */

#include "Quadtree.hpp"
#include <stdlib.h>
#include "Polygon.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <math.h>

PolyQuadTree::PolyQuadTree(Vec2 lower_bound,Vec2 upper_bound,int max_depth){
	this->max_depth=max_depth;
	max_nodes=2;
	for(int i=0;i<max_depth;i++)
		max_nodes*=4;
	next_node=0;
	tree=new PolyQuadNode[max_nodes];
	make_node(lower_bound,upper_bound,0);
}

PolyQuadTree::~PolyQuadTree(){
	delete [] tree;
}

void PolyQuadTree::ClearTree(){
	Vec2 lower_bound=tree[0].lower_bound,upper_bound=tree[0].upper_bound;
	next_node=0;
	make_node(lower_bound,upper_bound,0);
	count=0;
}

void PolyQuadTree::ClearTree(Vec2 lower_bound,Vec2 upper_bound){
	next_node=0;
	make_node(lower_bound,upper_bound,0);
	count=0;
}

unsigned int PolyQuadTree::make_node(Vec2 lower_bound,Vec2 upper_bound,int depth){

	tree[next_node].lower_bound=lower_bound;
	tree[next_node].upper_bound=upper_bound;
	tree[next_node].depth=depth;
	for(int i=0;i<4;i++)
		tree[next_node].children[i]=0;
	tree[next_node].contained_polygons.clear();
	next_node++;
	return next_node-1;

}

bool PolyQuadTree::isContained(Polygon * poly,unsigned int node){
	if(tree[node].lower_bound.x>poly->BoundingBox[1].x || poly->BoundingBox[0].x>tree[node].upper_bound.x || tree[node].lower_bound.y>poly->BoundingBox[1].y || poly->BoundingBox[0].y>tree[node].upper_bound.y)
		return false;
	else
		return true;
}

bool PolyQuadTree::isContained(Vec2 lower_bound,Vec2 upper_bound,unsigned int node){
	if(tree[node].lower_bound.x>upper_bound.x || lower_bound.x>tree[node].upper_bound.x || tree[node].lower_bound.y>upper_bound.y || lower_bound.y>tree[node].upper_bound.y)
		return false;
	else
		return true;
}

void PolyQuadTree::Draw(unsigned int node){
	GLint current_program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&current_program);
	bool go=true;
	glUniform3f(glGetUniformLocation(current_program,"color"),0.0f,0.0f,1.0f);
	if(go){
		std::vector<GLfloat> vertex;
		vertex.push_back(tree[node].lower_bound.x);
		vertex.push_back(tree[node].lower_bound.y);
		vertex.push_back(tree[node].upper_bound.x);
		vertex.push_back(tree[node].lower_bound.y);
		vertex.push_back(tree[node].upper_bound.x);
		vertex.push_back(tree[node].upper_bound.y);
		vertex.push_back(tree[node].lower_bound.x);
		vertex.push_back(tree[node].upper_bound.y);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertex.data());
		glDrawArrays( GL_LINE_LOOP,0,vertex.size()/2);
	}
	if(tree[node].children[0]!=0){
		for(int i=0;i<4;i++){
			Draw(tree[node].children[i]);
		}
	}
}

void PolyQuadTree::Insert(Polygon * polygon,std::vector<Polygon *> & potential_collisions,unsigned int node){
	if(isContained(polygon,node)){
		if(node==0){
			count++;
		}
		if(tree[node].children[0]!=0){
			for(int i=0;i<4;i++){
				Insert(polygon,potential_collisions,tree[node].children[i]);
			}
		}
		else{
			if(tree[node].depth==max_depth){
				for(int i=0;i<tree[node].contained_polygons.size();i++){
					bool add=true;
					for(int j=0;j<potential_collisions.size();j++){
						if(potential_collisions[j]==tree[node].contained_polygons[i]){
							add=false;
							break;
						}
					}
					if(add){
						potential_collisions.push_back(tree[node].contained_polygons[i]);
					}
				}
				tree[node].contained_polygons.push_back(polygon);
			}
			else if(tree[node].contained_polygons.size()>0){
				Vec2 diff=(tree[node].upper_bound-tree[node].lower_bound)*0.5;
				//std::vector<Polygon * > temppoly=tree[node].contained_polygons;
				tree[node].children[0]=make_node(Vec2(tree[node].lower_bound.x+diff.x,tree[node].lower_bound.y),Vec2(tree[node].upper_bound.x,tree[node].upper_bound.y-diff.y),tree[node].depth+1);
				tree[node].children[1]=make_node(tree[node].lower_bound,tree[node].upper_bound-diff,tree[node].depth+1);
				tree[node].children[2]=make_node(Vec2(tree[node].lower_bound.x,tree[node].lower_bound.y+diff.y),Vec2(tree[node].upper_bound.x-diff.x,tree[node].upper_bound.y),tree[node].depth+1);
				tree[node].children[3]=make_node(tree[node].lower_bound+diff,tree[node].upper_bound,tree[node].depth+1);
				for(int i=0;i<4;i++){
					for(int j=0;j<tree[node].contained_polygons.size();j++){
						Insert(tree[node].contained_polygons[j],potential_collisions,tree[node].children[i]);
					}
					Insert(polygon,potential_collisions,tree[node].children[i]);
				}
				tree[node].contained_polygons.clear();

			}
			else{
				tree[node].contained_polygons.push_back(polygon);
			}
		}
	}
}

void PolyQuadTree::Insert(Polygon * polygon,unsigned int node){
	if(isContained(polygon,node)){
		if(node==0){
			count++;
		}
		if(tree[node].children[0]!=0){
			for(int i=0;i<4;i++){
				Insert(polygon,tree[node].children[i]);
			}
		}
		else{
			if(tree[node].depth==max_depth){
				tree[node].contained_polygons.push_back(polygon);
			}
			else if(tree[node].contained_polygons.size()>0){
				Vec2 diff=(tree[node].upper_bound-tree[node].lower_bound)*0.5;
				//std::vector<Polygon * > temppoly=tree[node].contained_polygons;
				tree[node].children[0]=make_node(Vec2(tree[node].lower_bound.x+diff.x,tree[node].lower_bound.y),Vec2(tree[node].upper_bound.x,tree[node].upper_bound.y-diff.y),tree[node].depth+1);
				tree[node].children[1]=make_node(tree[node].lower_bound,tree[node].upper_bound-diff,tree[node].depth+1);
				tree[node].children[2]=make_node(Vec2(tree[node].lower_bound.x,tree[node].lower_bound.y+diff.y),Vec2(tree[node].upper_bound.x-diff.x,tree[node].upper_bound.y),tree[node].depth+1);
				tree[node].children[3]=make_node(tree[node].lower_bound+diff,tree[node].upper_bound,tree[node].depth+1);
				for(int i=0;i<4;i++){
					for(int j=0;j<tree[node].contained_polygons.size();j++){
						Insert(tree[node].contained_polygons[j],tree[node].children[i]);
					}
					Insert(polygon,tree[node].children[i]);
				}
				tree[node].contained_polygons.clear();

			}
			else{
				tree[node].contained_polygons.push_back(polygon);
			}
		}
	}
}

void PolyQuadTree::LookUp(Polygon * polygon,std::vector<Polygon *> & potential_collisions,unsigned int node){
	if(isContained(polygon,node)){
		if(tree[node].children[0]!=0){
			for(int i=0;i<4;i++){
				LookUp(polygon,potential_collisions,tree[node].children[i]);
			}
		}
		else{
			for(int i=0;i<tree[node].contained_polygons.size();i++){
				if(tree[node].contained_polygons[i]->master!=polygon->master){
					bool add=true;
					for(int j=0;j<potential_collisions.size();j++){
						if(potential_collisions[j]==tree[node].contained_polygons[i]){
							add=false;
							break;
						}
					}
					if(add){
						potential_collisions.push_back(tree[node].contained_polygons[i]);
					}
				}
			}
		}
	}
}

