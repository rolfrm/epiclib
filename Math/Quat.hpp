/*
 * Quat.hpp
 *
 *  Created on: Oct 6, 2011
 *      Author: sebastian
 */

#ifndef QUAT_HPP_
#define QUAT_HPP_

class Quaternion{
public:
	Quaternion(float ax,float ay,float az,float aw);

	void operator =(Quaternion);
	Quaternion operator * (Quaternion);

	void normalize();

	void Write2Matrix(float * MatrixArray);

	void print();

	float x,y,z,w;
};

#endif /* QUAT_HPP_ */
