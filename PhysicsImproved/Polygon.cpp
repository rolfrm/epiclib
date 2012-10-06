#include <math.h>

#include "Polygon.hpp"


void Triangle::calcNormalsAndEdges(){
	Normal[0]=Vertex[1]-Vertex[0];
	Normal[1]=Vertex[2]-Vertex[1];
	Normal[2]=Vertex[0]-Vertex[2];
	
	for(int i=0;i<3;i++){
		Edge[i]=Normal[i];
		Normal[i]/=Normal[i].getNorm2();
		Normal[i].setValue(-Normal[i].y,Normal[i].x);
	}

}

Vec2 Triangle::getCenterOfMass(){
	Vec2 return_vec;
	
	for(int i=0;i<3;i++)
		return_vec+=Vertex[i]/3;
		
	return return_vec;

}

void Triangle::calcAreaAndInertia(){
	// It is assummed that the triangle's density function is constant and mass is unit
	Vec2 vb=Vertex[1]-Vertex[0];
	Vec2 vb_unit=vb/vb.getNorm2();
	Vec2 vb_unit_orth=Vec2(-vb_unit.y,vb_unit.x);
	Vec2 vc=Vertex[2]-Vertex[0];
	
	double b=vb.getNorm2();
	double a=fabs(vb_unit*vc);
	double h=fabs(vb_unit_orth*vc);
	
	inertia=h*pow(b,3)+h*a*pow(b,2)+h*pow(a,2)*a+b*pow(h,3);
	inertia/=12.0;
	
	area=b*h*0.5;
}

bool Triangle::pointInside(Vec2 p){
	Vec2 v0=-Edge[2];
	Vec2 v1=Edge[0];
	Vec2 v2=p-Vertex[0];
	
	double dot00=v0*v0;
	double dot01=v0*v1;
	double dot02=v0*v2;
	double dot11=v1*v1;
	double dot12=v1*v2;
	
	double invDenom = 1.0/(dot00*dot11-dot01*dot01);
	double u = (dot11*dot02-dot01*dot12)*invDenom;
	double v = (dot00*dot12-dot01*dot02)*invDenom;
	
	return (u>=0) && (v>=0) && (u+v<1);

}

bool Polygon::isEar(std::vector<Vec2> & vertex_list,unsigned int index){

	return false;
}
