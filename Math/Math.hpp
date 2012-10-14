/*
 * Math.hpp
 *
 *  Created on: Aug 8, 2011
 *      Author: sebastian
 */

#ifndef MATH_HPP_
#define MATH_HPP_

class Vec2{
public:
  Vec2(double x,double y);
  Vec2();

  inline void operator += (const Vec2 & V){x+=V.x,y+=V.y;};
  inline void operator *= (double v){x*=v,y*=v;};
  inline void operator -= (const Vec2 & V){x-=V.x,y-=V.y;};
  inline void operator /= (double v){x/=v,y/=v;};;
  inline Vec2 operator + (const Vec2 & V){return Vec2(x+V.x,y+V.y);};
  inline Vec2 operator - (const Vec2 & V){return Vec2(x-V.x,y-V.y);};
  inline Vec2 operator - (){return Vec2(-x,-y);};
  inline Vec2 operator * (double v){return Vec2(v*x,v*y);};
  inline Vec2 operator / (double v){return Vec2(x/v,y/v);};;
  inline double operator * (const Vec2 & V){return V.x*x+V.y*y;};
  inline void operator = (const Vec2 & V){x=V.x,y=V.y;};
  void setValue(double nx,double ny){x=nx;y=ny;};

  double getNorm2();
  double getNorm2Squared();
  
  void print();
  
  double x,y;
};

double Crossproduct(Vec2,Vec2);
bool LineIntersection(Vec2 point1,Vec2 edge1,Vec2 point2,Vec2 edge2);
bool LineIntersection(Vec2 point1,Vec2 edge1,Vec2 point2,Vec2 edge2,Vec2 * intersection);
double Randomd(double lower_limit,double upper_limit,int samples);

#endif /* MATH_HPP_ */
