
/*
 * ShadeCore.hpp
 *
 *  Created on: Sep 27, 2011
 *      Author: sebastian
 */

#ifndef SHADECORE_HPP_
#define SHADECORE_HPP_

#include "Color.h"
#include <GL/glew.h>
#include <math.h>
#include <IL/il.h>
#include <GL/gl.h>
#include <GL/glfw.h>

	struct vec3{
		vec3(GLfloat x,GLfloat y,GLfloat z){
			this->x=x,this->y=y,this->z=z;
		}
		GLfloat x,y,z;
	};

void initOpenGL(unsigned int n_width,unsigned int n_height,float n_framerate=60.0f);
void terminateOpenGL();
void ClearBuffer(Colorf color);

void setFramerate(float n_framerate);
void SwapBuffers();
int UpdateGraphics();

void GenerateObject2World(GLfloat * mat,vec3 position,vec3 angle,vec3 scale);
void GenerateWorld2View(GLfloat * mat,vec3 position,vec3 angle);
void GenerateView2Perspective(GLfloat * mat,GLfloat width,GLfloat height,GLfloat near,GLfloat far);

unsigned int getWidth();
unsigned int getHeight();


#endif /* SHADECORE_HPP_ */
