/*
 * Quat.cpp
 *
 *  Created on: Oct 6, 2011
 *      Author: sebastian
 */

#include "Quat.hpp"
#include <math.h>
#include <iostream>

Quaternion::Quaternion(float ax,float ay,float az,float aw){
	x=ax;
	y=ay;
	z=az;
	w=aw;
}

void Quaternion::operator =(Quaternion Q){
	x=Q.x;
	y=Q.y;
	z=Q.z;
	w=Q.w;
}

Quaternion Quaternion::operator *(Quaternion Q){
	Quaternion out(1,0,0,0);
	out.w=w*Q.w-x*Q.x-y*Q.y-z*Q.z;
	out.x=w*Q.x+x*Q.w+y*Q.z-z*Q.y;
	out.y=w*Q.y-x*Q.z+y*Q.w+z*Q.x;
	out.z=w*Q.z+x*Q.y-y*Q.x+z*Q.w;
	return out;
}


void Quaternion::normalize(){
	float mag=sqrt(x*x+y*y+z*z+w*w);
	w/=mag;
	x/=mag;
	y/=mag;
	z/=mag;
}

void Quaternion::print(){
	std::cout<<"x="<<x<<"\ny="<<y<<"\nz="<<z<<"\nw="<<w<<"\n";
}


void Quaternion::Write2Matrix(float * MatrixArray){
	/*(*M)<<1-2*Q.y*Q.y-2*Q.z*Q.z<<2*Q.x*Q.y-2*Q.w*Q.z<<2*Q.x*Q.z+2*Q.w*Q.y<<0<<arma::endr
			<<2*Q.x*Q.y+2*Q.w*Q.z<<1-2*Q.x*Q.x-2*Q.z*Q.z<<2*Q.y*Q.z-2*Q.w*Q.x<<0<<arma::endr
			<<2*Q.x*Q.z-2*Q.w*Q.y<<2*Q.y*Q.z+2*Q.w*Q.x<<1-2*Q.x*Q.x-2*Q.y*Q.y<<0<<arma::endr
			<<0<<0<<0<<1;*/
	MatrixArray[0]=1-2*y*y-2*z*z,MatrixArray[4]=2*x*y-2*w*z,MatrixArray[8]=2*x*z+2*w*y,MatrixArray[12]=0;
	MatrixArray[1]=2*x*y+2*w*z,MatrixArray[5]=1-2*x*x-2*z*z,MatrixArray[9]=2*y*z-2*w*x,MatrixArray[13]=0;
	MatrixArray[2]=2*x*z-2*w*y,MatrixArray[6]=2*y*z+2*w*x,MatrixArray[10]=1-2*x*x-2*y*y,MatrixArray[14]=0;
	MatrixArray[3]=0,MatrixArray[7]=0,MatrixArray[11]=0,MatrixArray[15]=1;
}
