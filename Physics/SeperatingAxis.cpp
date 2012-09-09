#include "Polygon.h"
#include "PhysicsObject.h"
#include "PhysicsCore.h"
#include "SeperatingAxis.hpp"
#include <iostream>
#include <math.h>
#include "Quadtree.hpp"
#include <GL/glfw.h>


	SeperatingAxis::SeperatingAxis(PhysicsCore * nCore){
		core=nCore;
		quad_tree=new PolyQuadTree(Vec2(-8000,-6000)*2,Vec2(8000,6000),5); //this area of focus should be adjustable
		fps_bounds=0;
		fps_detect=0;
	}

	SeperatingAxis::~SeperatingAxis(){
		delete quad_tree;
	}

	void SeperatingAxis::Run(){

		double time=glfwGetTime();
		for(std::list<PhysicsObject *>::iterator it=core->Objects.begin();it!=core->Objects.end();it++){
			for(std::list<Polygon>::iterator itf=(*it)->Body.begin();itf!=(*it)->Body.end();itf++){
				itf->FindBounds();
			}
		}
		fps_bounds=fps_bounds*0.95+0.05/(glfwGetTime()-time);
/*

		for(std::list<Dormir::PhysicsObject *>::iterator it=core->Objects.begin();it!=core->Objects.end();it++){
			std::list<Dormir::PhysicsObject *>::iterator it2=it;
			for(it2++;it2!=core->Objects.end();it2++){
				if((*it)->GetMass()!=0 || (*it2)->GetMass()!=0){
					CollisionDetection(*it,*it2);
				}
			}
		}

*/
		time=glfwGetTime();
		quad_tree->ClearTree();
		std::vector<Polygon * > potential_collisions;
		for(std::list<PhysicsObject *>::iterator it=core->Objects.begin();it!=core->Objects.end();it++){
			for(std::list<Polygon>::iterator itpoly=(*it)->Body.begin();itpoly!=(*it)->Body.end();itpoly++){
				potential_collisions.clear();
				//quad_tree->LookUp(&(*itpoly));
				quad_tree->Insert(&(*itpoly),potential_collisions);
				for(int i=0;i<potential_collisions.size();i++){
					if(itpoly->master->GetMass() !=0 || potential_collisions[i]->master->GetMass() !=0){
						if(PreCollisionDetection(&(*itpoly),potential_collisions[i])){
							SAT(&(*itpoly),potential_collisions[i],itpoly->master,potential_collisions[i]->master);
						}
					}
				}
			}
		}
		fps_detect=fps_detect*0.95+0.05/(glfwGetTime()-time);
	//	std::cout<<"collision detection times, bounds = "<<fps_bounds<<", detection = "<<fps_detect<<"\n";
	//	std::cout<<"objects contained in tree = "<<quad_tree->count<<"\n";

	//	quad_tree->Draw();

	}


	void SeperatingAxis::CollisionDetection(PhysicsObject * obj1,PhysicsObject * obj2){
		for(std::list<Polygon>::iterator it=obj1->Body.begin();it!=obj1->Body.end();it++){
			for(std::list<Polygon>::iterator it2=obj2->Body.begin();it2!=obj2->Body.end();it2++){
				if(PreCollisionDetection(&*it,&*it2)){
					SAT(&*it,&*it2,obj1,obj2);
				}
			}
		}
	}

	void SeperatingAxis::CollisionDetection(Polygon * poly,std::vector<CollisionNode> &){
		poly->FindBounds();
		std::vector<Polygon *> temp_out;
		quad_tree->LookUp(poly,temp_out);

	}


	void SeperatingAxis::SAT(Polygon * P1,Polygon * P2,PhysicsObject * obj1,PhysicsObject * obj2){

			CollisionNode Node,Node2;
			Vec2 Contact1,Contact2;
			Node.intersection=0;
			bool first=true;
			double cMax1,cMin2;
			for(unsigned int w=0;w<2;w++){

				for(unsigned int i=0;i<P1->Axis.size();i++){
					double min2=P1->Axis[i]*P2->Vertex[0],max1=P1->Axis[i]*P1->Vertex[0];
					for(unsigned int j=1;j<P1->Vertex.size();j++){
						double temp=P1->Axis[i]*P1->Vertex[j];
						if(temp>max1){
							max1=temp;
						}
					}
					for(unsigned int j=1;j<P2->Vertex.size();j++){
						double temp=P1->Axis[i]*P2->Vertex[j];
						if(temp<min2){
							min2=temp;
						}
					}

					if(min2>=max1)
						return;
					else if(max1-min2<Node.intersection || first){
						Node.intersection=max1-min2; //Proportion of intersectionev
						Node.axis=P1->Axis[i]; //Axis of the collision
						Node.from=obj1; //Reference to the object from which the axis originates
						Node.to=obj2; //Reference to the opposing object
						Node.poly_from=P1;
						Node.poly_to=P2;
						cMax1=max1;
						cMin2=min2;
						first=false;
					}

				}
				PhysicsObject * temp=obj1;
				obj1=obj2,obj2=temp;
				Polygon * tempP=P1;
				P1=P2,P2=tempP;
			}
			if(Node.from==obj2){
				PhysicsObject * temp=obj1;
				obj1=obj2,obj2=temp;
				Polygon * tempP=P1;
				P1=P2,P2=tempP;
			}



			unsigned int index2=0,I2[2];
			for(unsigned int j=0;j<P2->Vertex.size();j++){
				double temp=P2->Vertex[j]*Node.axis;
				double ntemp=P2->Vertex[(j+1)%P2->Vertex.size()]*Node.axis;
				if(temp<=cMax1 && ntemp>=cMax1){
					I2[1]=j;
					index2++;
				}
				else if(temp>=cMax1 && ntemp<=cMax1){
					I2[0]=(j+1)%P2->Vertex.size();
					index2++;
				}
			}

			unsigned int index1=0,I1[2];
			for(unsigned int j=0;j<P1->Vertex.size();j++){
				double temp=P1->Vertex[j]*Node.axis;
				double ntemp=P1->Vertex[(j+1)%P1->Vertex.size()]*Node.axis;
				if(temp>=cMin2 && ntemp<=cMin2){
					I1[1]=j;
					index1++;
				}
				else if(temp<=cMin2 && ntemp>=cMin2){
					I1[0]=(j+1)%P1->Vertex.size();
					index1++;
				}
			}
			if(index1==0 || index2==0)
				return;
			if(index2==1){
			Vec2 V2[3];
			V2[1]=P2->Vertex[I2[0]];

			if(I2[0]==0)
				V2[0]=P2->Vertex[P2->Vertex.size()-1];
			else
				V2[0]=P2->Vertex[I2[0]-1];

			if(I2[0]==P2->Vertex.size()-1)
				V2[2]=P2->Vertex[0];
			else
				V2[2]=P2->Vertex[I2[0]+1];
		//	arma::mat::fixed<2,2> V1;
			Vec2 V1[2];
			V1[0]=P1->Vertex[I1[0]];
			V1[1]=P1->Vertex[I1[1]];
			FindIntersection(V1[0],V1[1],V2[0],V2[1],&Contact1);
			//arma::vec2 temp;
			FindIntersection(V1[0],V1[1],V2[2],V2[1],&Contact2);
			//Node.contact+=temp;
			//Node.contact/=2;
			}
			else{

				Vec2 Edge;
				Edge.SetValue(-Node.axis.y,Node.axis.x);
				Vec2 V1[2],V2[2];

				V1[0]=P1->Vertex[I1[0]];
				V1[1]=P1->Vertex[I1[1]];

				V2[0]=P2->Vertex[I2[0]];
				V2[1]=P2->Vertex[I2[1]];
				double PEM1[2],PEM2[2];
				PEM1[0]=V1[0]*Edge;
				PEM1[1]=V1[1]*Edge;
				PEM2[0]=V2[0]*Edge;
				PEM2[1]=V2[1]*Edge;

				if(PEM1[0]>PEM1[1]){
					double temp=PEM1[0];
					PEM1[0]=PEM1[1],PEM1[1]=temp;
				}

				if(PEM2[0]>PEM2[1]){
					double temp=PEM2[0];
					PEM2[0]=PEM2[1],PEM2[1]=temp;
				}


				if(PEM1[0]<=PEM2[0] && PEM2[1]<=PEM1[1]){
					unsigned int n2;
					if(I2[0]==0)
						n2=P2->Vertex.size()-1;
					else
						n2=I2[0]-1;
					FindIntersection(P1->Vertex[I1[0]],P1->Vertex[I1[1]],P2->Vertex[I2[0]],P2->Vertex[n2],&Contact1);
					if(I2[1]==P2->Vertex.size()-1)
						n2=0;
					else
						n2=I2[1]+1;
					FindIntersection(P1->Vertex[I1[0]],P1->Vertex[I1[1]],P2->Vertex[I2[1]],P2->Vertex[n2],&Contact2);
				}
				else if(PEM1[0]<=PEM2[0] && PEM1[1]<PEM2[1]){
					unsigned int n1;
					if(I1[1]==P1->Vertex.size()-1)
						n1=0;
					else
						n1=I1[1]+1;
					FindIntersection(P2->Vertex[I2[0]],P2->Vertex[I2[1]],P1->Vertex[I1[1]],P1->Vertex[n1],&Contact1);
					unsigned int n2;
					if(I2[1]==P2->Vertex.size()-1)
						n2=0;
					else
						n2=I2[1]+1;
					FindIntersection(P1->Vertex[I1[0]],P1->Vertex[I1[1]],P2->Vertex[I2[1]],P2->Vertex[n2],&Contact2);

				}
				else if(PEM2[0]<PEM1[0] && PEM2[1]<=PEM1[1]){

					unsigned int n2;
					if(I2[0]==0)
						n2=P2->Vertex.size()-1;
					else
						n2=I2[0]-1;
					FindIntersection(P1->Vertex[I1[0]],P1->Vertex[I1[1]],P2->Vertex[I2[0]],P2->Vertex[n2],&Contact1);
					unsigned int n1;
					if(I1[0]==0)
						n1=P1->Vertex.size()-1;
					else
						n1=I1[0]-1;
					FindIntersection(P2->Vertex[I2[1]],P2->Vertex[I2[0]],P1->Vertex[I1[0]],P1->Vertex[n1],&Contact2);
				}
				else{
					unsigned int n1;
					if(I1[0]==0)
						n1=P1->Vertex.size()-1;
					else
						n1=I1[0]-1;
					FindIntersection(P2->Vertex[I2[0]],P2->Vertex[I2[1]],P1->Vertex[I1[0]],P1->Vertex[n1],&Contact1);
					if(I1[1]==P1->Vertex.size()-1)
						n1=0;
					else
						n1=I1[1]+1;
					FindIntersection(P2->Vertex[I2[0]],P2->Vertex[I2[1]],P1->Vertex[I1[1]],P1->Vertex[n1],&Contact2);
				}

			}

			Node.tangent.SetValue(-Node.axis.y,Node.axis.x);
			Node.contact[0].point=Contact1;
			Node.contact[1].point=Contact2;


			//if(arma::norm(Contact1-Contact2,2)>1e-1){


			//std::cout<<index1<<" "<<index2<<" found contacts\n";
			//Contact1.Print();
		//	Contact2.Print();

			/*Node2=Node;
			Node2.contact=Contact2;
			Node2.rf=Node2.contact-Node2.from->Pos;
			Node2.rt=Node2.contact-Node2.to->Pos;

			core->AddCollisionNode(Node2);
			*/
			Node.contact[0].arm_from=Contact1-Node.from->Pos;
			Node.contact[0].arm_to=Contact1-Node.to->Pos;
			Node.contact[1].arm_from=Contact2-Node.from->Pos;
			Node.contact[1].arm_to=Contact2-Node.to->Pos;
			core->AddCollisionNode(Node);

		}

	bool SeperatingAxis::PreCollisionDetection(Polygon * obj1,Polygon * obj2){
		//arma::mat::fixed<2,2> B1=obj1->getBound(),B2=obj2->getBound();
		//if(B2(0,0)>B1(0,1) || B1(0,0)>B2(0,1) || B2(1,0)>B1(1,1) || B1(1,0)>B2(1,1))
		//	return false;
		if(obj2->BoundingBox[0].x>obj1->BoundingBox[1].x || obj1->BoundingBox[0].x>obj2->BoundingBox[1].x || obj2->BoundingBox[0].y>obj1->BoundingBox[1].y || obj1->BoundingBox[0].y>obj2->BoundingBox[1].y)
			return false;
		return true;
	}

	void SeperatingAxis::FindIntersection(Vec2 P1,Vec2 P2,Vec2 P3,Vec2 P4,Vec2 * C){
		(*C).SetValue(((P1.x*P2.y-P1.y*P2.x)*(P3.x-P4.x)-(P1.x-P2.x)*(P3.x*P4.y-P3.y*P4.x))/((P1.x-P2.x)*(P3.y-P4.y)-(P1.y-P2.y)*(P3.x-P4.x))
				,((P1.x*P2.y-P1.y*P2.x)*(P3.y-P4.y)-(P1.y-P2.y)*(P3.x*P4.y-P3.y*P4.x))/((P1.x-P2.x)*(P3.y-P4.y)-(P1.y-P2.y)*(P3.x-P4.x)));
	}

	double SeperatingAxis::FindMax(double Values[],unsigned int size){
		double temp=Values[0];
		for(unsigned int i=1;i<size;i++){
			if(temp<Values[i])
				temp=Values[i];
		}
		return temp;
	}

	double SeperatingAxis::FindMin(double Values[],unsigned int size){
		double temp=Values[0];
		for(unsigned int i=1;i<size;i++){
			if(temp>Values[i])
				temp=Values[i];
		}
		return temp;
	}

