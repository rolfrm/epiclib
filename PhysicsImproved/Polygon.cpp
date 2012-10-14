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

void Triangle::calcBoundingCircle()
{
  Center.setValue(0,0);
  
  for(int i=0;i<3;i++)
    Center+=Vertex[i];
  
  Center/=3.0;
  
  radius=0;
  
  for(int i=0;i<3;i++){
    double temp=Vertex[i]-Center;
    if(temp>radius)
      radius=temp;
  }
}


bool Polygon::isEar(std::vector<Vec2> & vertex_list,unsigned int index){
	unsigned int prev_i=index-1,next_i=(index+1)%vertex_list.size();
	if(index==0)
	 	prev_i=vertex_list.size()-1;
	
	Vec2 v1=vertex_list[prev_i]-vertex_list[index],v2=vertex_list[next_i]-vertex_list[index];
	
	if(Crossproduct(v1,v2)>=0.0)
	   return false;
	 	
	Triangle triangle(vertex_list[prev_i],vertex_list[index],vertex_list[next_i]);
	
	for(unsigned int i=(index+2)%vertex_list.size();i!=prev_i;i=(i+1)%vertex_list.size()){
		if(triangle.pointInside(vertex_list[i]))
			return false;
	}
	
	return true;
}

void Polygon::decompose(){
	if(!isClockwise())
		reverseVertex();
	
	
	std::vector<Vec2> vertex_list = complete_vertex_vector;
	triangle_mesh.clear();
	
	while(vertex_list.size()>2){
		unsigned int index=0;
		for(int i=0;i<vertex_list.size();i++){
			if(isEar(vertex_list,i)){
				index=i;
				break;
			}
		}
		unsigned int prev_i=index-1,next_i=(index+1)%vertex_list.size();
		if(index==0)
	 		prev_i=vertex_list.size()-1;
	 		
		triangle_mesh.push_back(Triangle(vertex_list[prev_i],vertex_list[index],vertex_list[next_i]));
		
		vertex_list.erase(vertex_list.begin()+index);
	
	}

}

bool Polygon::isClockwise()
{
  	double Area=0;
	for(unsigned int i=0;i<complete_vertex_vector.size()-1;i++)
		Area+=Crossproduct(complete_vertex_vector[i],complete_vertex_vector[i+1]);
	
	Area+=Crossproduct(complete_vertex_vector[complete_vertex_vector.size()-1],complete_vertex_vector[0]);
// 	Area/=2;
	if(Area<0.0)
	   return false;
	
	return true;
}

void Polygon::reverseVertex()
{
  	std::vector<Vec2> Temp=complete_vertex_vector;
	for(unsigned int i=0;i<complete_vertex_vector.size();i++){
		complete_vertex_vector[i].x=Temp[complete_vertex_vector.size()-(i+1)].x;
		complete_vertex_vector[i].y=Temp[complete_vertex_vector.size()-(i+1)].y;
	}
}
