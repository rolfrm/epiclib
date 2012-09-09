/*
 * PhysicsObject.h
 *
 *  Created on: Feb 10, 2011
 *      Author: sebastian
 */

//#include "../../Utils/VectorMath.h"
#include <list>
#include <map>
#include "Math/Math.hpp"
#include "Polygon.h"

#ifndef PHYSICSOBJECT_H_
#define PHYSICSOBJECT_H_
#define TOLERANCE 1e-10


	class PhysicsCore;

	class SeperatingAxis;

	class LCPSolver;

	class Polygon;

	struct CollisionNode;

	class Fixpoint;

	
	

	class PhysicsObject{
	public:
		PhysicsObject(Vec2 Pos,double e,double mu,double mass);
		PhysicsObject(Vec2 Pos,double e,double mu);
		PhysicsObject(Vec2);
		PhysicsObject(double mass,double e=0,double mu=0.6);
		PhysicsObject();

		void Advance();
		void Revert();
		void AdjustPosition(Vec2 V);
		void AdjustPosition(double x,double y);
		void SetPosition(Vec2 V);
		void SetPosition(double x,double y){AdjustPosition(x-Pos.x,y-Pos.y);};
		void SetAngle(double a){AdjustAngle(a-angle);};
		void AdjustAngle(double a);
		void AddForce(Vec2 Force,Vec2 Arm);
		void AddForce(Vec2 Force);
		void AddForce(double x,double y);
		void AddTorque(double T){angleSpeed+=T*invInertia;};

		bool LoadPolygon(Polygon P);
		bool LoadPolygon(double * vertex_list,unsigned int n_vertex);
		void LoadPolygonList(std::list<Polygon>);
		bool AttachPoint(Fixpoint * F);
		bool DetachPoint(Fixpoint * F);
		void DeleteBody();
		double GetMass(){return mass;};
		double GetInverseMass(){return invMass;};
		double GetInverseInertia(){return invInertia;};
		double GetInertia(){return inertia;};
		double GetAnglespeed(){return angleSpeed;};
		double GetAngle(){return angle;};
		double GetRestitution(){return e;};
		double GetFriction(){return mu;};
		Vec2 GetVelocity(){return Vel;};
		Vec2 GetPosition(){return Pos;};
		void setMass(double newMass);
		void setInertia(double newInertia);
		void setRestitution(double newE){e=newE;};
		void setFriction(double newMu){mu=newMu;};
		void setAnglespeed(double a){angleSpeed=a;};
		void setVelocity(double x,double y){Vel.x=x;Vel.y=y;};
		void FindBounds();
		void UpdateGeometricData();
		void CalculateMomentofInertia();
		//void SetGhost(bool isGhost);
		//bool IsGhost();
		int GetID();
		void SetID(int nID){ID=nID;};

		virtual void OnCollision(CollisionNode *){};

		friend class SeperatingAxis;
		friend class LCPSolver;
		friend class Model;
		friend class Impulse;
		friend class Core;
		std::list<Polygon> Body;
		std::list<Fixpoint *> AttachedPoints;
		std::multimap<PhysicsObject *,CollisionNode *> PreviousCollisions;

//	protected:
		void Init(Vec2 Pos,double e,double mu,double mass);
		void AdvancePosition();
		void AdvanceAngle();
		void RevertPosition();
		void RevertAngle();

		Vec2 Pos,Vel;
		double e,mu,mass,invMass,invInertia,inertia,angle,angleSpeed;
		PhysicsCore * World;
		//bool ghost;
		
		int ID;
		static int next_id;
	};




#endif /* PHYSICSOBJECT_H_ */
