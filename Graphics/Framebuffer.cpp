/*
 * Framebuffer.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: sebastian
 */

#include "Framebuffer.hpp"
#include "Texture.hpp"



FrameBuffer::FrameBuffer(GLuint width,GLuint height,GLuint internal_format,GLuint texture_copies,GLuint interpolation,GLuint wrap){
	render_buffer=new Texture2D(width,height,internal_format,texture_copies,interpolation,wrap);

	glGenFramebuffers(1,&reference);
	BindFramebuffer();

	for(GLuint i=0;i<render_buffer->n_tex;i++){
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,render_buffer->reference[i],0);
	}

	UnbindFramebuffer();

	count=new GLuint;
	*count=1;
}

FrameBuffer::FrameBuffer(const FrameBuffer & original){
	render_buffer=new Texture2D(*original.render_buffer);
	reference=original.reference;
	count=original.count;
	*count+=1;
}

FrameBuffer::~FrameBuffer(){
	*count-=1;
	if(*count==0){
		delete count;
		glDeleteFramebuffers(1,&reference);
		delete render_buffer;
	}
}



void FrameBuffer::BindFramebuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER,reference);
}

void FrameBuffer::UnbindFramebuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::ClearRenderBuffer(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha){
	glClearColor(red,green,blue,alpha);
	glClear( GL_COLOR_BUFFER_BIT);
}

