/*
 * Framebuffer.hpp
 *
 *  Created on: Jul 30, 2012
 *      Author: sebastian
 */

#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_
#include "../Utils/SharedPtr.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <map>
#include "Texture.hpp"

class frameBufferObject{
  int ref;
public:
  frameBufferObject();
  frameBufferObject(int gl_ref);
  int & GetGLRef();
  void Dispose();
};


class FrameBuffer{
  SharedPtr<frameBufferObject> fbo;
  void genRenderBuffer(int width, int height, 
		      PixelFormat iFormat, 
		      Interpolation ipol, 
		       TextureWrap wrap, 
		       int attachmentType,
		       DataFormat tdtype = DataFormat::RGBA);
  static FrameBuffer current_buffer;
  
public:
  static FrameBuffer screenBuffer;
  FrameBuffer();

  void addBuffer(int width ,int height, PixelFormat internalFormat, 
		 Interpolation interpolation = Interpolation::Nearest,
		 TextureWrap wrap = TextureWrap::ClampToEdge);

  
  void addColorBuffer(int width,int height,int channel,
		      PixelFormat internal_format,
		      Interpolation interpolation=Interpolation::Nearest,
		      TextureWrap wrap=TextureWrap::ClampToEdge);
  

  void addDepthStencilBuffer(int width,int height,
			     PixelFormat internalFormat,
			     Interpolation interpolation=Interpolation::Nearest,
			     TextureWrap wrap=TextureWrap::ClampToEdge);
  

  void bindFrameBuffer();
  void Bind();
  static void bindScreenBuffer();
  
  static void clearColorBuffer(GLfloat red=0.0f,GLfloat green=0.0f,
			       GLfloat blue=0.0f,GLfloat alpha=0.0f);
  static void clearDepthBuffer(){glClear( GL_DEPTH_BUFFER_BIT);}
  void Clear();
  int depth_stencil_attachment;
  Texture depth_stencil;
  std::map<int, Texture> render_buffers;
};

#endif /* FRAMEBUFFER_HPP_ */
