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
	bindFramebuffer();
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	bindScreenbuffer();

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
	bindFramebuffer();
	
	glDrawBuffer(GL_FRONT);
	glReadBuffer(GL_FRONT);
	
	Texture tempTex(width,height,NULL,interpolation,wrap,internal_format);
	
	for(GLuint i=0;i<tempTex.n_tex;i++){
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i+render_buffers.size(),GL_TEXTURE_2D,tempTex.unsafeOpenGLTextureRef(),0);
	}
	
	
	render_buffers.push_back(tempTex);
	
	bindScreenbuffer();
	
}

void FrameBuffer::addDepthBuffer(GLuint width,GLuint height,PixelFormat internal_format,Interpolation interpolation,TextureWrap wrap){
	bindFramebuffer();
	
	depth_buffer=Texture(width,height,NULL,interpolation,wrap,internal_format);;
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depth_buffer.unsafeOpenGLTextureRef(),0);
	
	
	GLenum FBOstatus=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE){
		throw "Error creating FBO";
	}
	
	bindScreenbuffer();

}

void FrameBuffer::addDepthStencilBuffer(GLuint width,GLuint height,Interpolation interpolation,TextureWrap wrap){
	bindFramebuffer();
	
	depth_buffer=Texture(width,height,NULL,interpolation,wrap,PixelFormat::Depth24Stencil8);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depth_buffer.unsafeOpenGLTextureRef(),0);
	
	
	GLenum FBOstatus=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout<<"Cannot make shadow map\n";
	
	bindScreenbuffer();

}


void FrameBuffer::bindFramebuffer(){
  glBindFramebuffer(GL_FRAMEBUFFER,reference);
  if(render_buffers.size()!=0)
	glViewport(0,0,render_buffers[0].width,render_buffers[0].height);
  else if(depth_buffer.width!=0)
    glViewport(0,0,depth_buffer.width,depth_buffer.height);
}

void FrameBuffer::bindScreenbuffer(){
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glViewport(0,0,getWidth(),getHeight());
}

void FrameBuffer::ClearColorBuffer(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha){
  glClearColor(red,green,blue,alpha);
  glClear( GL_COLOR_BUFFER_BIT);
}
