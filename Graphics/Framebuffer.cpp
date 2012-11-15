/*
 * Framebuffer.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: sebastian
 */


#include <GL/glew.h>
#include <GL/gl.h>

#include "Framebuffer.hpp"
#include "GraphicsCore.hpp"

FrameBuffer::FrameBuffer(){
	glGenFramebuffers(1,&reference);
	bindFrameBuffer();
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	//addDepthBuffer(800,800);
	
	bindScreenBuffer();

	count=new GLuint;
	*count=1;
	
	
}

/*
FrameBuffer::FrameBuffer(GLuint width,GLuint height,GLuint internal_format,GLuint texture_copies,GLuint interpolation,GLuint wrap){
  render_buffer=new Texture2D(width,height,internal_format,texture_copies,interpolation,wrap);
  
  glGenFramebuffers(1,&reference);
  BindFramebuffer();

  for(GLuint i=0;i<render_buffer->n_tex;i++){
	  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,render_buffer->reference[i],0);
  }

  BindScreenBuffer();

  count=new GLuint;
  *count=1;
}
*/

FrameBuffer::FrameBuffer(const FrameBuffer & original){
  render_buffers=original.render_buffers;
  depth_buffer=original.depth_buffer;
  stencil_buffer=original.stencil_buffer;
  reference=original.reference;
  count=original.count;
  *count+=1;
}

FrameBuffer::~FrameBuffer(){
  *count-=1;
  if(*count==0){
    delete count;
    glDeleteFramebuffers(1,&reference);
  }
}

void FrameBuffer::addColorBuffer(GLuint width,GLuint height,PixelFormat internal_format,Interpolation interpolation,TextureWrap wrap){
	bindFrameBuffer();
	
	if(render_buffers.size()==0){
		glDrawBuffer(36064);
		glReadBuffer(36064);
	}
	
	Texture tempTex(width,height,NULL,interpolation,wrap,internal_format);
	
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+render_buffers.size(),GL_TEXTURE_2D,tempTex.unsafeOpenGLTextureRef(),0);	

	GLenum FBOstatus=glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout<<"Cannot attach color buffer\n";
		
	
	
	render_buffers.push_back(tempTex);
	
	std::vector<GLenum> draw_buffers;
	
	for(int i=0;i<render_buffers.size();i++){
		draw_buffers.push_back(GL_COLOR_ATTACHMENT0+i);
	}
	
	glDrawBuffers(draw_buffers.size(),draw_buffers.data());
	
	bindScreenBuffer();
	
}

void FrameBuffer::addDepthBuffer(GLuint width,GLuint height,PixelFormat internal_format,Interpolation interpolation,TextureWrap wrap){
	bindFrameBuffer();
	
	
	
	depth_buffer=Texture(width,height,NULL,interpolation,wrap,internal_format,DataFormat::Depth);
	depth_buffer.Unbind(0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depth_buffer.unsafeOpenGLTextureRef(),0);
	
	GLenum FBOstatus=glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout<<"Cannot attach depth buffer\n";
	
	bindScreenBuffer();

}


void FrameBuffer::bindFrameBuffer(){
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,reference);

  
  if(render_buffers.size()!=0){
  	
	glViewport(0,0,render_buffers[0].width,render_buffers[0].height);
	
	}
  else if(depth_buffer.width!=0)
    glViewport(0,0,depth_buffer.width,depth_buffer.height);
}

void FrameBuffer::bindScreenBuffer(){
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glViewport(0,0,getWidth(),getHeight());
}

void FrameBuffer::clearColorBuffer(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha){
  glClearColor(red,green,blue,alpha);
  glClear( GL_COLOR_BUFFER_BIT);
}
