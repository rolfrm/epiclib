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

frameBufferObject::frameBufferObject(){
  ref = -1;
}

frameBufferObject::frameBufferObject(int gl_ref){
  ref = gl_ref;
}

int & frameBufferObject::GetGLRef(){
  return ref;
}

void frameBufferObject::Dispose(){
  unsigned int uref = ref;
  glDeleteFramebuffers(1,&uref);
}


FrameBuffer::FrameBuffer(){
  depth_stencil_attachment = -1;
}


void FrameBuffer::genRenderBuffer(int width, int height, 
		      PixelFormat iFormat, 
		      Interpolation ipol, 
				  TextureWrap wrap,int attachmentType,
				  DataFormat tdtype){

  if(fbo.Get().GetGLRef() == -1){
    unsigned int ref;
    glGenFramebuffers(1,&ref);
    fbo = frameBufferObject(ref);
  }
  
  bindFrameBuffer();
  
  Texture tempTex(width,height,NULL,ipol,wrap,iFormat,tdtype);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,attachmentType,GL_TEXTURE_2D,tempTex.unsafeOpenGLTextureRef(),0);
  GLenum FBOstatus=glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if(FBOstatus != GL_FRAMEBUFFER_COMPLETE){
    std::cout << FBOstatus << "\n";
    throw "Error creating FBO";
  }
  if(attachmentType == GL_DEPTH_ATTACHMENT || attachmentType == GL_DEPTH_STENCIL_ATTACHMENT || attachmentType == GL_STENCIL_ATTACHMENT){
    depth_stencil = tempTex;
    depth_stencil_attachment = attachmentType;
  }else{
    int idx = attachmentType - GL_COLOR_ATTACHMENT0;
    render_buffers[idx] = tempTex;
  }
}

void FrameBuffer::addDepthStencilBuffer(int width ,int height, PixelFormat internalFormat, 
		 Interpolation interpolation,
			    TextureWrap wrap){

  
  int attachmentType = -1;
  DataFormat dformat = DataFormat::Depth;
  switch(internalFormat){
  case(PixelFormat::Depth):
  case(PixelFormat::Depth16):
  case(PixelFormat::Depth24):
  case(PixelFormat::Depth32):
    
    attachmentType = GL_DEPTH_ATTACHMENT;break;
  case(PixelFormat::Depth24Stencil8):
    dformat = DataFormat::DepthStencil;
    attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;break;
  case(PixelFormat::Stencil8):
    attachmentType = GL_STENCIL_ATTACHMENT;break;
  }
  if(attachmentType == -1){
    throw "Wrong attachment type";
  }
  genRenderBuffer(width,height,internalFormat,interpolation,wrap,attachmentType,dformat);
}
void FrameBuffer::addColorBuffer(int width ,int height, int channel, 
				 PixelFormat internalFormat, 
				 Interpolation interpolation,
				 TextureWrap wrap){

  
  genRenderBuffer(width,height,internalFormat,interpolation,wrap,GL_COLOR_ATTACHMENT0 + channel);

}



void FrameBuffer::bindFrameBuffer(){
  unsigned int glRef = fbo.Get().GetGLRef();
  /*if(current_buffer.fbo.Get().GetGLRef() == glRef){
    return;
    }*/
  
  if(glRef == -1){
    bindScreenBuffer();
    current_buffer = *this;
    return;
  }
  
  unsigned int buffers[20];
  int count = 0;
  if(depth_stencil_attachment != -1){
    buffers[count++] = depth_stencil_attachment; 
  }
  for(auto it = render_buffers.begin();it != render_buffers.end();it++){
    int key = it->first;
    buffers[count++] = GL_COLOR_ATTACHMENT0 + it->first;
  }
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo.Get().GetGLRef());
   if(count != 0){
     //glDrawBuffers(count,buffers);
   }

  if(render_buffers.size()!=0){
  	
    glViewport(0,0,render_buffers[0].width,render_buffers[0].height);
	
  }
  else if(depth_stencil.width!=0){
    glViewport(0,0,depth_stencil.width,depth_stencil.height);
  }
  /*if(render_buffers.size()==0){
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    }*/
  current_buffer = *this;
}

void FrameBuffer::bindScreenBuffer(){
  
  //unsigned int buffers[] = {GL_FRONT_AND_BACK};
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  /*glDrawBuffers(1,buffers);
  glReadBuffer(GL_FRONT_AND_BACK);
  glDrawBuffer(GL_FRONT_AND_BACK);*/
  glViewport(0,0,getWidth(),getHeight());
}

void FrameBuffer::clearColorBuffer(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha){
  glClearColor(red,green,blue,alpha);
  glClear( GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::Bind(){
  bindFrameBuffer();
}

void FrameBuffer::Clear(){
  glClearColor(0,0,0,0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


FrameBuffer FrameBuffer::current_buffer;
FrameBuffer FrameBuffer::screenBuffer;
