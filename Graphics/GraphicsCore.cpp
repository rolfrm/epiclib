/*
 * ShadeCore.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: sebastian
 */


#include <GL/glew.h>
#include "GraphicsCore.hpp"
#include <math.h>
#include <IL/il.h>
#include <GL/gl.h>
#include <GL/glfw.h>

GLuint Width,Height;
GLfloat framerate,inv_framerate;
double last_time;

GLuint getWidth(){
	return Width;
}

GLuint getHeight(){
	return Height;
}



void initOpenGL(GLuint n_width,GLuint n_height,GLfloat n_framerate){
	Width=n_width;
	Height=n_height;

	glfwInit();
	glfwOpenWindow(Width,Height,8,8,8,8,8,8,GLFW_WINDOW);
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


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

GLint UpdateGraphics(){
	glfwSwapBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClear( GL_COLOR_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);

	double temp=glfwGetTime()-last_time;
	if(inv_framerate-temp>0)
		glfwSleep(inv_framerate-temp);


	last_time+=temp;

	return !glfwGetKey( GLFW_KEY_ESC ) &&glfwGetWindowParam( GLFW_OPENED );
}

void GenerateObject2World(GLfloat * matrix,float dx,float dy,float da,float sx,float sy){
	for(unsigned int i=0;i<16;i++){
		matrix[i]=0;
	}
	matrix[0]=sx*cos(da),matrix[4]=-sx*sin(da),matrix[12]=dx,
			matrix[1]=sy*sin(da),matrix[5]=sy*cos(da),matrix[13]=dy;

	matrix[10]=1,matrix[15]=1;
}

void GenerateWorld2View(GLfloat * matrix,float dx,float dy,float da,float sx,float sy){
	for(unsigned int i=0;i<16;i++){
		matrix[i]=0;
	}
	matrix[0]=sx*cos(da),matrix[4]=-sx*sin(da),matrix[12]=sx*dx,
			matrix[1]=sy*sin(da),matrix[5]=sy*cos(da),matrix[13]=sy*dy;

	matrix[10]=1,matrix[15]=1;
}


void GenerateInverseWorld2View(GLfloat * matrix,float dx,float dy,float da,float sx,float sy){
	for(unsigned int i=0;i<16;i++){
		matrix[i]=0;
	}

	matrix[0]=cos(da)/sx,matrix[4]=-sin(da)/sy,matrix[12]=sin(da)*dy-cos(da)*dx;
	matrix[1]=sin(da)/sx,matrix[5]=cos(da)/sy,matrix[13]=-(sin(da)*dx+cos(da)*dy);
	matrix[10]=1,matrix[15]=1;
}


