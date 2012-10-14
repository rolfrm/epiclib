#include <vector>

#include "../Math/Math.hpp"


class Triangle{
public:
	Triangle(Vec2 v1,Vec2 v2,Vec2 v3){
		Vertex[0]=v1,Vertex[1]=v2,Vertex[2]=v3;
		calcNormalsAndEdges();
		calcAreaAndInertia();
		calcBoundingCircle();
	  
	}
	
	Triangle(const Triangle & original){
		for(int i=0;i<3;i++){
			Vertex[i]=original.Vertex[i];
			Normal[i]=original.Normal[i];
			Edge[i]=original.Edge[i];
		}
		inertia=original.inertia;
		area=original.area;
		Center=original.Center;
		radius=original.radius;
	}
	
	bool pointInside(Vec2 point);
	
	void calcNormalsAndEdges();
	
	Vec2 getCenterOfMass();
	
	void calcAreaAndInertia();
	
	void calcBoundingCircle();
	
	Vec2 Vertex[3];
	Vec2 Normal[3];
	Vec2 Edge[3];
	Vec2 Center;
	
	double inertia,area,radius;
};

class Polygon{
public:
	Polygon(){};
	Polygon(const Polygon & original){
		complete_vertex_vector=original.complete_vertex_vector;
		triangle_mesh=original.triangle_mesh;
	}
	Polygon(std::vector<Vec2> vertex_list){
		complete_vertex_vector=vertex_list;
	}
	Polygon(double * vertex_array,unsigned int count){
		for(unsigned int i=0;i<count;i++)
			complete_vertex_vector.push_back(Vec2(vertex_array[i*2],vertex_array[i*2+1]));
	}
	
	virtual void onCollision(){};
	
	void decompose();
	
	void addVertex(Vec2 v){
		complete_vertex_vector.push_back(v);
	}
	
	void addVertex(double x,double y){
		complete_vertex_vector.push_back(Vec2(x,y));
	}
	
	bool isEar(std::vector<Vec2> & vertex_list,unsigned int index);
	
	bool isClockwise();
	
	void reverseVertex();
	
	std::vector<Vec2> complete_vertex_vector;
	std::vector<Triangle> triangle_mesh;
};
