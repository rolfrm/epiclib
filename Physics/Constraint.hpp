/*
 * Constraint.hpp
 *
 *  Created on: Jul 26, 2011
 *      Author: sebastian
 */

#ifndef CONSTRAINT_HPP_
#define CONSTRAINT_HPP_

#include "Math/Math.h"


class PhysicsObject;

class Fixpoint{
public:
	Fixpoint(){};

	void setPhysicsObject(PhysicsObject * O);
	PhysicsObject * getPhysicsObject(){return Obj;};

	void SetPos(Vec2 V){P=V;};
	void AdjustPos(Vec2 V){P+=V;};
	Vec2 getPos(){return P;};
	void Rotate(double angle);

private:
	Vec2 P;
	PhysicsObject * Obj;
};

class Constraint{
public:
	Constraint(){};

	virtual void ApplyForce()=0;
};

class Spring:public Constraint{
public:
	Spring(double springconstant,double dampening,double rest);

	void ApplyForce();


//private:
	double k,d,x;
	Fixpoint F[2];
};


class SingleSprint:public Constraint{
public:
	SingleSprint(double springconstant,double dampening,double rest);

	void ApplyForce();

	void SetFirstObject(PhysicsObject * nO1){O1=nO1;};

	PhysicsObject * getObject(){return O1;};

	void SetSpringConstant(double nk){k=nk;};

	void setSpringDampening(double nd){d=nd;};

	void setSprintRest(double nX);

	void SetFixedPos(Vec2 P){FixedPos=P;};
	void SetFixedPos(double x,double y){FixedPos.SetValue(x,y);};
	void SetFixedPos(int x,int y){FixedPos.SetValue(x,y);};

	Vec2 GetFixedPos(){return FixedPos;};

private:
	double k,d,x;
	PhysicsObject * O1;
	Vec2 FixedPos;
};

class Joint{
public:
	Joint(){v_bias=0.2;};
	Joint(PhysicsObject * O1,double x1,double y1,PhysicsObject * O2,double x2,double y2,double nSlop=0);
	Joint(PhysicsObject * O1,PhysicsObject * O2,double x,double y,double nSlop=0);
	~Joint();

	void setSlop(double nSlop);

	Fixpoint P[2];
	double v_bias,slop;
	Vec2 AccummulatedImpulse;
};



#endif /* CONSTRAINT_HPP_ */
