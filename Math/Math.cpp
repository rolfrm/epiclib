/*
 * Math.cpp
 *
 *  Created on: Aug 8, 2011
 *      Author: sebastian
 */

#include "Math.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>


	Vec2::Vec2(){
		setValue(0,0);
	}

	Vec2::Vec2(double x,double y){
		setValue(x,y);
	}

	double Vec2::getNorm2(){
		return sqrt(x*x+y*y);
	}

double Vec2::GetNorm2(){
  return getNorm2();
}

	double Vec2::getNorm2Squared(){
		return x*x+y*y;
	}


	double Crossproduct(Vec2 V,Vec2 U){
		return V.x*U.y-V.y*U.x;
	}

	void Vec2::print(){
		std::cout<<"("<<x<<","<<y<<")\n";
	}


	bool LineIntersection(Vec2 point1,Vec2 edge1,Vec2 point2,Vec2 edge2){
		double det=edge2.y*edge1.x-edge2.x*edge1.y;
		if(fabs(det)<1e-15)
			return false;
		double s=edge2.x*(point1.y-point2.y)-edge2.y*(point1.x-point2.x);
		s/=det;
		double t=edge1.x*(point1.y-point2.y)-edge1.y*(point1.x-point2.x);
		t/=det;
		if(t>=0 && t<1 && s>=0 && s<1){
			return true;
		}
		else
			return false;
	}

	bool LineIntersection(Vec2 point1,Vec2 edge1,Vec2 point2,Vec2 edge2,Vec2 * intersection){
		double det=edge2.y*edge1.x-edge2.x*edge1.y;
		if(fabs(det)<1e-15)
			return false;
		double s=edge2.x*(point1.y-point2.y)-edge2.y*(point1.x-point2.x);
		s/=det;
		double t=edge1.x*(point1.y-point2.y)-edge1.y*(point1.x-point2.x);
		t/=det;
		intersection->x=point1.x+s*edge1.x;
		intersection->y=point1.y+s*edge1.y;
		if(t>=0 && t<1 && s>=0 && s<1){
			return true;
		}
		else
			return false;
	}

	double Randomd(double lower_limit,double upper_limit,int samples){
		double random=(double)(rand()%samples);
		random/=samples;
		return lower_limit+(upper_limit-lower_limit)*random;
	}



