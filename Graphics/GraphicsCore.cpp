/*
 * ShadeCore.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: sebastian
 */

#include "GraphicsCore.hpp"



#include "../Math/Quat.h"

GLuint Width,Height;
GLfloat framerate,inv_framerate;
double last_time;

GLuint getWidth(){
	return Width;
}

GLuint getHeight(){
	return Height;
}

void ClearBuffer(Colorf color){
  glClearColor(color.data[0],color.data[1],color.data[2],color.data[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void initOpenGL(GLuint n_width,GLuint n_height,GLfloat n_framerate){
	Width=n_width;
	Height=n_height;

	glfwInit();
	glfwOpenWindow(Width,Height,8,8,8,8,8,8,GLFW_WINDOW);
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	ilInit();

	glEnable(GL_TEXTURE_2D);
	setFramerate(n_framerate);
	last_time=glfwGetTime();
	
}

void terminateOpenGL(){
	glfwTerminate();
}


void setFramerate(GLfloat n_framerate){
	framerate=n_framerate;
	inv_framerate=1.0f/n_framerate;
}

void SwapBuffers(){
  glfwSwapBuffers();
}

GLint UpdateGraphics(){
	glfwSwapBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear( GL_COLOR_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);
	glClear( GL_DEPTH_BUFFER_BIT);

	double temp=glfwGetTime()-last_time;
	if(inv_framerate-temp>0)
		glfwSleep(inv_framerate-temp);


	last_time+=temp;

	return !glfwGetKey( GLFW_KEY_ESC ) &&glfwGetWindowParam( GLFW_OPENED );
}


