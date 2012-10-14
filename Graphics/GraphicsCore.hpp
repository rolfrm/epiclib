/*
 * ShadeCore.hpp
 *
 *  Created on: Sep 27, 2011
 *      Author: sebastian
 */

#ifndef SHADECORE_HPP_
#define SHADECORE_HPP_

#include "Color.h"

void initOpenGL(unsigned int n_width,unsigned int n_height,float n_framerate=60.0f);
void terminateOpenGL();
void ClearBuffer(Color color);

void setFramerate(float n_framerate);
void SwapBuffers();
int UpdateGraphics();

void GenerateObject2World(float * matrix,float dx,float dy,float da,float sx,float sy);
void GenerateWorld2View(float * matrix,float dx,float dy,float da,float sx,float sy);
void GenerateInverseWorld2View(float * matrix,float dx,float dy,float da,float sx,float sy);

unsigned int getWidth();
unsigned int getHeight();


#endif /* SHADECORE_HPP_ */
