#include "Math/Math.hpp"

class IntersectionSolver;
	class PhysicsCore;
	class PhysicsObject;
	class Polygon;
	class PolyQuadTree;


	struct sContactpoint{
		Vec2 point,arm_from,arm_to;
		double I,T;
	};


	class CollisionNode{
		public:
			PhysicsObject * from,* to;
			Vec2 axis,tangent;
			sContactpoint contact[2];
			double intersection;
			Polygon * poly_from,* poly_to;
	};


	class SeperatingAxis{
	public:
		SeperatingAxis(PhysicsCore * nCore);
		~SeperatingAxis();
		void Run();
		void FindIntersection(Vec2 P1,Vec2 P2,Vec2 P3,Vec2 P4,Vec2 * C);
		void CollisionDetection(Polygon *,std::vector<CollisionNode> &);
	private:
		void CollisionDetection(PhysicsObject * obj1,PhysicsObject * obj2);
		void SAT(Polygon * P1,Polygon * P2,PhysicsObject * obj1,PhysicsObject * obj2);
		inline bool PreCollisionDetection(Polygon * obj1,Polygon * obj2);
		double FindMax(double [],unsigned int);
		double FindMin(double [],unsigned int);
		PhysicsCore * core;
		PolyQuadTree * quad_tree;
		double fps_bounds,fps_detect;
	};


