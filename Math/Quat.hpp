/*
 * Quat.hpp
 *
 *  Created on: Oct 6, 2011
 *      Author: sebastian
 */

#ifndef QUAT_HPP_
#define QUAT_HPP_

#include <GL/gl.h>

class Quaternion{
public:
	Quaternion(GLfloat ax,GLfloat ay,GLfloat az,GLfloat aw);

	void operator =(Quaternion);
	Quaternion operator * (Quaternion);

	void normalize();

	void Write2Matrix(GLfloat * MatrixArray);

	void print();

	GLfloat x,y,z,w;
};

#endif /* QUAT_HPP_ */
