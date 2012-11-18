/*
 * Framebuffer.hpp
 *
 *  Created on: Jul 30, 2012
 *      Author: sebastian
 */

#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

#include "Texture.hpp"

class FrameBuffer{
public:
  FrameBuffer();
  FrameBuffer(const FrameBuffer & original);
  ~FrameBuffer();
  
  void addColorBuffer(GLuint width,GLuint height,PixelFormat internal_format,Interpolation interpolation=Interpolation::Nearest,TextureWrap wrap=TextureWrap::ClampToEdge);
  
  void addDepthBuffer(GLuint width,GLuint height,PixelFormat internal_format=PixelFormat::Depth,Interpolation interpolation=Interpolation::Nearest,TextureWrap wrap=TextureWrap::ClampToEdge);
  
  void addDepthStencilBuffer(GLuint width,GLuint height,Interpolation interpolation=Interpolation::Nearest,TextureWrap wrap=TextureWrap::ClampToEdge);
  

  void addStencilBuffer(GLuint width,GLuint height,Interpolation interpolation=Interpolation::Nearest,TextureWrap wrap=TextureWrap::ClampToEdge);
  
  
  void bindFrameBuffer();
  static void bindScreenBuffer();
  
  static void clearColorBuffer(GLfloat red=0.0f,GLfloat green=0.0f,GLfloat blue=0.0f,GLfloat alpha=0.0f);
  static void clearDepthBuffer(){glClear( GL_DEPTH_BUFFER_BIT);}
  
  Texture stencil_buffer,depth_buffer;
  std::vector<Texture> render_buffers;
  GLuint * count,reference;
};

#endif /* FRAMEBUFFER_HPP_ */
