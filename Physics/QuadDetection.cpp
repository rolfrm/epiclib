/*
 * QuadDetection.cpp
 *
 *  Created on: Jul 19, 2012
 *      Author: sebastian
 */

#include "QuadDetection.hpp"
#include <stdlib.h>
#include "Quadtree.hpp"
#include "Polygon.h"
#include "PhysicsObject.h"
#include <math.h>
#include <iostream>


QuadtreeCollisionDetecter::QuadtreeCollisionDetecter(){
	quad_tree=new PolyQuadTree(Vec2(-400,-300),Vec2(400,300),8);
}

QuadtreeCollisionDetecter::~QuadtreeCollisionDetecter(){
	delete quad_tree;
}

void QuadtreeCollisionDetecter::GenerateTree(){
	output.clear();

	quad_tree->ClearTree();
	for(std::list<PhysicsObject * >::iterator itobj=active_objects.begin();itobj!=active_objects.end();itobj++){
		//(*itobj)->UpdateGeometricData();
		(*itobj)->Advance();
		(*itobj)->FindBounds();
		for(std::list<Polygon>::iterator it=(*itobj)->Body.begin();it!=(*itobj)->Body.end();it++){
			//std::vector<Dormir::Polygon *> clist;
			//quad_tree->Insert(&(*it),clist);
			CollisionDetection(&(*it),true);
		}
	}
	//std::cout<<"we got "<<output.size()<<"\n";
}

void QuadtreeCollisionDetecter::CollisionDetection(Polygon * input,bool insert){
	std::vector<Polygon *> potential_collisions;
	quad_tree->Insert(input,potential_collisions);
	for(int i=0;i<potential_collisions.size();i++){
		if(input->master->GetMass()!=0 || potential_collisions[i]->master->GetMass()!=0){
			for(int j=0;j<potential_collisions[i]->line_segments.size();j++){
				for(int k=0;k<input->line_segments.size();k++){
					collision_node return_node;
					LineCollisionDetection(&potential_collisions[i]->line_segments[j],&input->line_segments[k],return_node);
					if(return_node.overlap>0){
						if(return_node.from==(PhysicsObject *)1){
							return_node.from=potential_collisions[i]->master;
							return_node.from_edge=j;
							return_node.to=input->master;
							return_node.to_edge=k;
						}
						else{
							return_node.to=potential_collisions[i]->master;
							return_node.to_edge=j;
							return_node.from=input->master;
							return_node.from_edge=k;
						}
						return_node.arm_from=return_node.contact-return_node.from->GetPosition();
						return_node.arm_to=return_node.contact-return_node.to->GetPosition();
						output.push_back(return_node);
					}
				}
			}
		}
	}


}

collision_node QuadtreeCollisionDetecter::LineCollisionDetection(LineSegment * line1,LineSegment * line2,collision_node & return_node){
	return_node.overlap=-1;
	if(LineIntersection(line1->vertex,line1->edge,line2->vertex, line2->edge,&return_node.contact)){
		SAT(line1->normal,line2->normal,line1->parent,line2->parent,return_node.normal,return_node.overlap,&return_node.from);
		return_node.tangent=Vec2(-return_node.normal.y,return_node.normal.x);
	}
	return return_node;
}

void QuadtreeCollisionDetecter::SAT(Vec2 Axis1,Vec2 Axis2,Polygon * poly1, Polygon * poly2,Vec2 & Axis,double & overlap,PhysicsObject ** signal){
	double max1=-INFINITY,min2=INFINITY;
	for(int i=0;i<poly1->Vertex.size();i++){
		double temp1=Axis1*poly1->Vertex[i];
		if(temp1>max1){
			max1=temp1;
		}
	}
	for(int i=0;i<poly2->Vertex.size();i++){
		double temp2=Axis1*poly2->Vertex[i];
		if(temp2<min2){
			min2=temp2;
		}
	}


	double min1=INFINITY,max2=-INFINITY;
	for(int i=0;i<poly1->Vertex.size();i++){
		double temp1=Axis2*poly1->Vertex[i];
		if(temp1<min1){
			min1=temp1;
		}
	}
	for(int i=0;i<poly2->Vertex.size();i++){
		double temp2=Axis2*poly2->Vertex[i];
		if(temp2>max2){
			max2=temp2;
		}
	}
	if(max1-min2<max2-min1){
		Axis=Axis1;
		overlap=max1-min2;
		*signal=(PhysicsObject *)1;
	}
	else{
		Axis= Axis2;
		overlap=max2-min1;
		*signal=(PhysicsObject *)2;
	}

}

