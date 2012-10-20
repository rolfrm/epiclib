/*
 * ShadeCore.cpp
 *
 *  Created on: Sep 27, 2011
 *      Author: sebastian
 */

#include "GraphicsCore.hpp"



#include "../Math/Quat.hpp"

GLuint Width,Height;
GLfloat framerate,inv_framerate;
double last_time;

GLuint getWidth(){
	return Width;
}

GLuint getHeight(){
	return Height;
}

void ClearBuffer(Color color){
  glClearColor(color.data[0],color.data[1],color.data[2],color.data[3]);
  glClear(GL_COLOR_BUFFER_BIT);
}


void initOpenGL(GLuint n_width,GLuint n_height,GLfloat n_framerate){
	Width=n_width;
	Height=n_height;

	glfwInit();
	glfwOpenWindow(Width,Height,8,8,8,8,8,8,GLFW_WINDOW);
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);


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
	glClear( GL_COLOR_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);
	glClear( GL_DEPTH_BUFFER_BIT);

	double temp=glfwGetTime()-last_time;
	if(inv_framerate-temp>0)
		glfwSleep(inv_framerate-temp);


	last_time+=temp;

	return !glfwGetKey( GLFW_KEY_ESC ) &&glfwGetWindowParam( GLFW_OPENED );
}

void GenerateView2Perspective(GLfloat * mat,GLfloat width,GLfloat height,GLfloat near,GLfloat far){
	for(int i=0;i<16;i++)
		mat[i]=0;
	
	mat[0]=near/width;
	mat[5]=near/height;
	mat[10]=-(near+far)/(far-near);
	mat[11]=-1;
	mat[14]=-2.0f*far*near/(far-near);
}

void GenerateWorld2View(GLfloat * mat,vec3 position,vec3 angle){
	for(int i=0;i<16;i++)
		mat[i]=0;
	
	Quaternion t1(sin(angle.x/2),0,0,cos(angle.x/2)),t2(0,sin(angle.y/2),0,cos(angle.y/2)),t3(0,0,sin(angle.z/2),cos(angle.z/2));
	Quaternion tot=t1*t2*t3;
	
	GLfloat temp[16];
	
	tot.Write2Matrix(temp);

	mat[0]=temp[0];
	mat[4]=temp[4];
	mat[8]=temp[8];
	mat[12]=position.x*temp[0]+position.y*temp[4]+position.z*temp[8];
	
	mat[1]=temp[1];
	mat[5]=temp[5];
	mat[9]=temp[9];
	mat[13]=position.x*temp[1]+position.y*temp[5]+position.z*temp[9];
	mat[2]=temp[2];
	mat[6]=temp[6];
	mat[10]=temp[10];
	mat[14]=position.x*temp[2]+position.y*temp[6]+position.z*temp[10];
	mat[3]=0;
	mat[7]=0;
	mat[11]=0;
	mat[15]=1;

}

void GenerateObject2World(GLfloat * mat,vec3 position,vec3 angle,vec3 scale){
	for(unsigned int i=0;i<16;i++)
		mat[i]=0;
	Quaternion t1(sin(angle.x/2),0,0,cos(angle.x/2)),t2(0,sin(angle.y/2),0,cos(angle.y/2)),t3(0,0,sin(angle.z/2),cos(angle.z/2));
	Quaternion tot=t1*t2*t3;


	GLfloat temp[16];

	tot.Write2Matrix(temp);

	mat[0]=temp[0],mat[4]=temp[4],mat[8]=temp[8],mat[12]=position.x;
	mat[1]=temp[1],mat[5]=temp[5],mat[9]=temp[9],mat[13]=position.y;
	mat[2]=temp[2],mat[6]=temp[6],mat[10]=temp[10],mat[14]=position.z;
	mat[3]=0,mat[7]=0,mat[11]=0,mat[15]=1;


}


