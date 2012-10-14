/*
 * ShadeCore.hpp
 *
 *  Created on: Sep 27, 2011
 *      Author: sebastian
 */

#ifndef SHADECORE_HPP_
#define SHADECORE_HPP_


#include <GL/gl.h>

void initOpenGL(GLuint n_width,GLuint n_height,GLfloat n_framerate=60.0f);
void terminateOpenGL();

void setFramerate(GLfloat n_framerate);
void SwapBuffers();
GLint UpdateGraphics();

void GenerateObject2World(GLfloat * matrix,float dx,float dy,float da,float sx,float sy);
void GenerateWorld2View(GLfloat * matrix,float dx,float dy,float da,float sx,float sy);
void GenerateInverseWorld2View(GLfloat * matrix,float dx,float dy,float da,float sx,float sy);

GLuint getWidth();
GLuint getHeight();


#endif /* SHADECORE_HPP_ */
