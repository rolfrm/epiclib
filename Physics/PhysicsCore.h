/*
 * PhysicsCore.h
 *
 *  Created on: Feb 10, 2011
 *      Author: sebastian
 */

#include <list>
#include <vector>
#include "Math/Math.h"

#ifndef PHYSICSCORE_H_
#define PHYSICSCORE_H_


	class PhysicsObject;
	class SeperatingAxis;
	class Model;
	class Constraint;
	class Joint;
	class Polygon;
	class CollisionNode;


	class PhysicsCore{
	public:
		PhysicsCore(unsigned int nMaxNodes=800);
		~PhysicsCore();

		void Run();

		bool LoadObject(PhysicsObject *);
		bool LoadConstraint(Constraint *);
		bool LoadJoint(Joint *);
		bool UnloadObject(PhysicsObject *);
		bool UnloadConstraint(Constraint *);
		bool UnloadJoint(Joint *);

		void AddCollisionNode(CollisionNode);

		void setGravity(double x,double y){Gravity.x=x,Gravity.y=y;};
		void setGravity(Vec2 nGravity){Gravity=nGravity;};

		Vec2 getGravity(){return Gravity;};

		void setVelocityDampening(double d){velocityDampening=d;};

		void setRotationDampening(double d){rotationDampening=d;};

		void setSlopOverlap(double d);
		void setBiasOverlap(double d){biasOverlap=Clamp(d,0.1,0.3);};
		double Clamp(double value,double lower,double upper);



		PhysicsObject * PointInsideObject(int x,int y);
		PhysicsObject * PointInsideObject(Vec2);



		friend class SeperatingAxis;
		friend class LCPSolver;
		friend class Model;
		friend class Impulse;
		friend class Intersection;
		friend class Contact;

//	protected:
		SeperatingAxis * CollisionDetecter;
		void AddImpulseNode(CollisionNode);
		double RoundDouble(double doValue, int nPrecision);
		double Max(double v1,double v2);
		void ExpandCollisionNodes(unsigned int);
		double CalculateImpulseCoeffiecient(Vec2 n,Vec2 rf,Vec2 rt,PhysicsObject * from,PhysicsObject * to);
		double CalculateRelativeVelocity(Vec2 n,Vec2 rf,Vec2 rt,PhysicsObject * from,PhysicsObject * to);

		std::vector<Constraint *> constraints;
		std::list<PhysicsObject *> Objects;
		std::vector<Joint *> Joints;
		CollisionNode * Nodes,* PreviousNodes;

		Vec2 Gravity;
		unsigned int allocatedNodes,allocatedPreviousNodes,maxNodes,runs;
		double velocityDampening,rotationDampening,slopOverlap,biasOverlap;
		double * A,* T,* K1,* K2,* K3;
		double n_AT,n_K;



		//std::list<CollisionNode> savedNodes;
	};



#endif /* PHYSICSCORE_H_ */
