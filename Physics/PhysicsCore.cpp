#include "PhysicsCore.h"
#include "SeperatingAxis.hpp"
#include "PhysicsObject.h"
#include "Constraint.hpp"
#include "Polygon.h"
#include <math.h>
#include <iostream>
#include <GL/glfw.h>




	double detect_time=0,resolve_time=0;

	PhysicsCore::PhysicsCore(unsigned int nMaxNodes){
		CollisionDetecter=new SeperatingAxis(this);
		allocatedNodes=0;
		allocatedPreviousNodes=0;
		maxNodes=nMaxNodes;
		Nodes = new CollisionNode[nMaxNodes];
		PreviousNodes = new CollisionNode[nMaxNodes];
		runs=0;

		setGravity(0,0);
		setVelocityDampening(0.01);
		setRotationDampening(0.01);
		setSlopOverlap(2);
		setBiasOverlap(0.2);

		A=new double[2];
		n_AT=2;
		T=new double[2];
		K1=new double[2];
		K2=new double[2];
		K3=new double[2];
		n_K=2;

	}

	PhysicsCore::~PhysicsCore(){
		delete CollisionDetecter;
		delete [] Nodes;
		delete [] A;
		delete [] T;
		delete [] K1;
		delete [] K2;
		delete [] K3;

	}

	void PhysicsCore::Run(){
				
		for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){
			(*it)->Advance();
		}

		double temp_time=glfwGetTime();
		CollisionDetecter->Run();
		detect_time=detect_time*0.95+0.05/(glfwGetTime()-temp_time);

		for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){

			(*it)->AddForce(Gravity*(*it)->GetMass());
			(*it)->AddForce((*it)->GetVelocity()*-velocityDampening);
			(*it)->AddTorque((*it)->GetAnglespeed()*-rotationDampening);
		}


		for(unsigned int i=0;i<constraints.size();i++)
			constraints[i]->ApplyForce();



		temp_time=glfwGetTime();
		if(n_AT<allocatedNodes*2){
			delete [] A;
			delete [] T;
			A = new double[allocatedNodes*2];
			T = new double[allocatedNodes*2];
			n_AT=allocatedNodes*2;

		}
		if(n_K<Joints.size()){
			K1= new double[Joints.size()];
			K2=new double[Joints.size()];
			K3 = new double[Joints.size()];
			n_K=Joints.size();
		}


		for(unsigned int i=0;i<allocatedNodes;i++){
			Nodes[i].contact[0].I=0;
			Nodes[i].contact[1].I=0;
			A[i*2]=CalculateImpulseCoeffiecient(Nodes[i].axis,Nodes[i].contact[0].arm_from,Nodes[i].contact[0].arm_to,Nodes[i].from,Nodes[i].to);
			A[i*2+1]=CalculateImpulseCoeffiecient(Nodes[i].axis,Nodes[i].contact[1].arm_from,Nodes[i].contact[1].arm_to,Nodes[i].from,Nodes[i].to);
			Nodes[i].contact[0].T=0;
			Nodes[i].contact[1].T=0;
			T[i*2]=CalculateImpulseCoeffiecient(Nodes[i].tangent,Nodes[i].contact[0].arm_from,Nodes[i].contact[0].arm_to,Nodes[i].from,Nodes[i].to);
			T[i*2+1]=CalculateImpulseCoeffiecient(Nodes[i].tangent,Nodes[i].contact[1].arm_from,Nodes[i].contact[1].arm_to,Nodes[i].from,Nodes[i].to);
		}
		for(unsigned int i=0;i<Joints.size();i++){
			PhysicsObject * O=Joints[i]->P[0].getPhysicsObject(),* O2=Joints[i]->P[1].getPhysicsObject();
			Vec2 r=Joints[i]->P[0].getPos(),r2=Joints[i]->P[1].getPos();
			double k=O->GetInverseMass()+r.GetNorm2()*r.GetNorm2()*O->GetInverseInertia();
			k+=O2->GetInverseMass()+r2.GetNorm2()*r2.GetNorm2()*O2->GetInverseInertia();
			K1[i]=k-(r.x*r.x*O->GetInverseInertia()+r2.x*r2.x*O2->GetInverseInertia());
			K2[i]=-(r.x*r.y*O->GetInverseInertia()+r2.x*r2.y*O2->GetInverseInertia());
			K3[i]=k-(r.y*r.y*O->GetInverseInertia()+r2.y*r2.y*O2->GetInverseInertia());
		}

		
		for(unsigned int i=0;i<allocatedNodes;i++){
			for(std::multimap<PhysicsObject *,CollisionNode *>::iterator it=Nodes[i].to->PreviousCollisions.equal_range(Nodes[i].from).first;it!=Nodes[i].to->PreviousCollisions.equal_range(Nodes[i].from).second;it++){
				CollisionNode PrevNode = *it->second;
				if((PrevNode.to==Nodes[i].to && PrevNode.from==Nodes[i].from) && PrevNode.poly_from==Nodes[i].poly_from && PrevNode.poly_to==Nodes[i].poly_to){
					Nodes[i].to->AddForce(Nodes[i].axis*PrevNode.contact[0].I,Nodes[i].contact[0].arm_to);
					Nodes[i].from->AddForce(Nodes[i].axis*-PrevNode.contact[0].I,Nodes[i].contact[0].arm_from);
					Nodes[i].contact[0].I=PrevNode.contact[0].I;
					Nodes[i].to->AddForce(Nodes[i].axis*PrevNode.contact[1].I,Nodes[i].contact[1].arm_to);
					Nodes[i].from->AddForce(Nodes[i].axis*-PrevNode.contact[1].I,Nodes[i].contact[1].arm_from);
					Nodes[i].contact[1].I=PrevNode.contact[1].I;

					Nodes[i].to->AddForce(Nodes[i].tangent*PrevNode.contact[0].T,Nodes[i].contact[0].arm_to);
					Nodes[i].from->AddForce(Nodes[i].tangent*-PrevNode.contact[0].T,Nodes[i].contact[0].arm_from);
					Nodes[i].contact[0].T=PrevNode.contact[0].T;
					Nodes[i].to->AddForce(Nodes[i].tangent*PrevNode.contact[1].T,Nodes[i].contact[1].arm_to);
					Nodes[i].from->AddForce(Nodes[i].tangent*-PrevNode.contact[1].T,Nodes[i].contact[1].arm_from);
					Nodes[i].contact[1].T=PrevNode.contact[1].T;

					break;
				}


			}
		}



		/*
		 * end
		 */

	
		double x_rel,y_rel,P,F;
		for(unsigned int l=0;l<5;l++){
			double N=0;



			for(unsigned int i=0;i<allocatedNodes;i++){
				double vbias=biasOverlap*Max(0,Nodes[i].intersection-slopOverlap);
				for(int j=0;j<2;j++){

					x_rel=-Nodes[i].contact[j].arm_to.y*Nodes[i].to->angleSpeed+Nodes[i].to->Vel.x-(-Nodes[i].contact[j].arm_from.y*Nodes[i].from->angleSpeed+Nodes[i].from->Vel.x);
					y_rel=Nodes[i].contact[j].arm_to.x*Nodes[i].to->angleSpeed+Nodes[i].to->Vel.y-(Nodes[i].contact[j].arm_from.x*Nodes[i].from->angleSpeed+Nodes[i].from->Vel.y);
					P=(-(Nodes[i].axis.x*x_rel+Nodes[i].axis.y*y_rel)+vbias)/A[i*2+j];
					double tempI=Nodes[i].contact[j].I;
					Nodes[i].contact[j].I+=P;
					if(Nodes[i].contact[j].I<0)
						Nodes[i].contact[j].I=0;



					Nodes[i].to->AddForce(Nodes[i].axis*(-tempI+Nodes[i].contact[j].I),Nodes[i].contact[j].arm_to);
					Nodes[i].from->AddForce(Nodes[i].axis*-(-tempI+Nodes[i].contact[j].I),Nodes[i].contact[j].arm_from);

					N+=fabs(-tempI+Nodes[i].contact[j].I);
					F=-(Nodes[i].tangent.x*x_rel+Nodes[i].tangent.y*y_rel)/T[i*2+j];
					double tempT=Nodes[i].contact[j].T;
					double u=sqrt(Nodes[i].to->GetFriction()*Nodes[i].from->GetFriction());
					Nodes[i].contact[j].T=Clamp(tempT+F,-u*Nodes[i].contact[j].I,u*Nodes[i].contact[j].I);
					Nodes[i].to->AddForce(Nodes[i].tangent*(Nodes[i].contact[j].T-tempT),Nodes[i].contact[j].arm_to);
					Nodes[i].from->AddForce(Nodes[i].tangent*-(Nodes[i].contact[j].T-tempT),Nodes[i].contact[j].arm_from);

				}

			}
			for(unsigned int i=0;i<Joints.size();i++){
				double k1=K1[i],k2=K2[i],k3=K3[i];
				PhysicsObject * O=Joints[i]->P[0].getPhysicsObject(),* O2=Joints[i]->P[1].getPhysicsObject();
				Vec2 P,dv=O->GetVelocity(),r=Joints[i]->P[0].getPos(),r2=Joints[i]->P[1].getPos();
				dv+=Vec2(-O->GetAnglespeed()*r.y,O->GetAnglespeed()*r.x);
				dv-=O2->GetVelocity();
				dv-=Vec2(-O2->GetAnglespeed()*r2.y,O2->GetAnglespeed()*r2.x);
				Vec2 dP=O->GetPosition()+Joints[i]->P[0].getPos()-O2->GetPosition()-Joints[i]->P[1].getPos();
				dP*=1-Joints[i]->slop/dP.GetNorm2();
				dP*=-Joints[i]->v_bias;
				dv-=dP;
				P.y=(-dv.y+dv.x*k2/k1)/(k3-k2*k2/k1);
				P.x=-(dv.x+P.y*k2)/k1;
				O->AddForce(P,r);
				O2->AddForce(P*(-1),r2);
				Joints[i]->AccummulatedImpulse+=P;
				N+=P.GetNorm2();
			}

			N/=allocatedNodes;
			if(N<1e-7){
				break;
			}
		}

		allocatedPreviousNodes=0;
		for(unsigned int i=0;i<allocatedNodes;i++){
			AddImpulseNode(Nodes[i]);
		}

		for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){
			(*it)->PreviousCollisions.clear();
		}

		for(unsigned int i=0;i<allocatedPreviousNodes;i++){
			PreviousNodes[i].from->PreviousCollisions.insert(std::pair<PhysicsObject*,CollisionNode *>(PreviousNodes[i].to,&PreviousNodes[i]));
			PreviousNodes[i].to->PreviousCollisions.insert(std::pair<PhysicsObject*,CollisionNode *>(PreviousNodes[i].from,&PreviousNodes[i]));
		}

		for(unsigned int i=0;i<allocatedNodes;i+=2){
			Nodes[i].to->OnCollision(&Nodes[i]);
			Nodes[i].from->OnCollision(&Nodes[i]);
		}

		allocatedNodes=0;
		resolve_time=resolve_time*0.95+0.05/(glfwGetTime()-temp_time);

		//std::cout<<"timers, detect = "<<detect_time<<", resovle = "<<resolve_time<<"\n";
			
	}

	void PhysicsCore::AddImpulseNode(CollisionNode N){
		if(allocatedPreviousNodes < maxNodes ){
			PreviousNodes[allocatedPreviousNodes]=N;
			allocatedPreviousNodes++;
		}
	}

	bool PhysicsCore::LoadObject(PhysicsObject * obj){
		for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){
			if(obj==*it)
				return false;
		}
		Objects.push_back(obj);
		return true;
	}

	bool PhysicsCore::LoadJoint(Joint * J){
		for(unsigned int i=0;i<Joints.size();i++){
			if(Joints[i]==J)
				return false;
		}
		Joints.push_back(J);
		return true;
	}



	bool PhysicsCore::UnloadObject(PhysicsObject * obj){
		for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){
			if(obj==*it){
				Objects.erase(it);
				return true;
			}
		}
		return false;

	}

	bool PhysicsCore::UnloadJoint(Joint * J){
		for(unsigned int i=0;i<Joints.size();i++){
			if(Joints[i]==J){
				Joints.erase(Joints.begin()+i);
				return true;
			}
		}
		return false;
	}



	void PhysicsCore::AddCollisionNode(CollisionNode N){
		if(allocatedNodes < maxNodes ){
			Nodes[allocatedNodes]=N;
			allocatedNodes++;
		}
		else
			ExpandCollisionNodes(maxNodes*2);
	}

	PhysicsObject * PhysicsCore::PointInsideObject(int x,int y){
		for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){
			for(std::list<Polygon>::iterator itf=(*it)->Body.begin();itf!=(*it)->Body.end();itf++){
				itf->FindBounds();
				if(itf->BoundingBox[0].x<x && x<itf->BoundingBox[1].x && itf->BoundingBox[0].y<y && y<itf->BoundingBox[1].y ){
					bool test=true;
					Vec2 Current=itf->Vertex[itf->Vertex.size()-1]-Vec2(x,y),Prev;
					for(int i=0;i<itf->Vertex.size();i++){
						Prev=Current;
						Current=itf->Vertex[i]-Vec2(x,y);
						if(Prev.x*Current.y-Prev.y*Current.x<0){
							test=false;
							break;
						}
						
					}
					if(test)
						return *it;

				}
			}
		}
		return NULL;
	}


	PhysicsObject * PhysicsCore::PointInsideObject(Vec2 v){
		return PointInsideObject((int)v.x,(int)v.y);
	}

	double PhysicsCore::RoundDouble(double doValue, int nPrecision){
		static const double doBase = 10.0;
		double doComplete5, doComplete5i;

		doComplete5 = doValue * pow(doBase, (double) (nPrecision + 1));

		if(doValue < 0.0)
		doComplete5 -= 5.0;
		else
		doComplete5 += 5.0;

		doComplete5 /= doBase;
		modf(doComplete5, &doComplete5i);

		return doComplete5i / pow(doBase, (double) nPrecision);
	}

	double PhysicsCore::Clamp(double value,double lower,double upper){
		if(value<=lower)
			return lower;
		else if(value>=upper)
			return upper;
		else
			return value;
	}



	void PhysicsCore::setSlopOverlap(double d){
		if(d>0)
			slopOverlap=d;
		else
			slopOverlap=0;
	}

	double PhysicsCore::Max(double v1,double v2){
		if(v1<v2)
			return v2;
		else
			return v1;
	}

	void PhysicsCore::ExpandCollisionNodes(unsigned int Count){
		if(Count>maxNodes){
			std::cout<<"expanding collision nodes with "<<Count<<"\n";
			CollisionNode * temp = new CollisionNode[Count];
			for(unsigned int i=0;i<allocatedNodes;i++)
				temp[i]=Nodes[i];
			delete [] Nodes;
			Nodes=temp;
			maxNodes=Count;

			temp=new CollisionNode[Count];

			for(unsigned int i=0;i<allocatedPreviousNodes;i++)
				temp[i]=PreviousNodes[i];


			delete [] PreviousNodes;
			PreviousNodes=temp;

			for(std::list<PhysicsObject *>::iterator it=Objects.begin();it!=Objects.end();it++){
				(*it)->PreviousCollisions.clear();
			}

			for(unsigned int i=0;i<allocatedPreviousNodes;i++){
				PreviousNodes[i].from->PreviousCollisions.insert(std::pair<PhysicsObject*,CollisionNode *>(PreviousNodes[i].to,&PreviousNodes[i]));
				PreviousNodes[i].to->PreviousCollisions.insert(std::pair<PhysicsObject*,CollisionNode *>(PreviousNodes[i].from,&PreviousNodes[i]));
			}

		}
	}

	bool PhysicsCore::LoadConstraint(Constraint * con){
		for(unsigned int i=0;i<constraints.size();i++)
			if(constraints[i]==con)
				return false;
		constraints.push_back(con);
		return true;
	}

	bool PhysicsCore::UnloadConstraint(Constraint * con){
		for(unsigned int i=0;i<constraints.size();i++)
			if(constraints[i]==con){
				constraints.erase(constraints.begin()+i);
				return true;
			}
		return false;
	}

	double PhysicsCore::CalculateImpulseCoeffiecient(Vec2 n,Vec2 rf,Vec2 rt,PhysicsObject * from,PhysicsObject * to){
		//		arma::vec2 temp=nj*obj->GetInverseMass()-(rj*(arma::dot(ri,nj))-nj*(arma::dot(ri,rj)))*obj->GetInverseInertia();
		//return from->GetInverseMass()+to->GetInverseMass()+Crossproduct(rf,n)*Crossproduct(rf,n)*from->GetInverseInertia()+Crossproduct(rt,n)*Crossproduct(rt,n)*to->GetInverseInertia();
		Vec2 temp=n*from->GetInverseMass()-(rf*(rf*n)-n*(rf*rf))*from->GetInverseInertia();
		temp+=n*to->GetInverseMass()-(rt*(rt*n)-n*(rt*rt))*to->GetInverseInertia();
		return temp*n;
	}

	double PhysicsCore::CalculateRelativeVelocity(Vec2 n,Vec2 rf,Vec2 rt,PhysicsObject * from,PhysicsObject * to){
		Vec2 temp2,temp3;
		temp2.SetValue(-rt.y*to->GetAnglespeed(),rt.x*to->GetAnglespeed());
		temp2+=to->GetVelocity();
		temp3.SetValue(-rf.y,rf.x);
		temp3*=from->GetAnglespeed();
		temp3+=from->GetVelocity();
		return n*(temp2-temp3);
	}



