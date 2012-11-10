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

class Texture2D;

class FrameBuffer{
public:
  FrameBuffer();
  FrameBuffer(GLuint width,GLuint height,GLuint internal_format,GLuint texture_copies=1,GLuint interpolation=GL_NEAREST,GLuint wrap=GL_CLAMP_TO_EDGE);
  FrameBuffer(const FrameBuffer & original);
  ~FrameBuffer();
  
  void BindFramebuffer();
  static void BindScreenBuffer();
  
  static void ClearColorBuffer(GLfloat red=0.0f,GLfloat green=0.0f,GLfloat blue=0.0f,GLfloat alpha=0.0f);
  
  Texture2D * render_buffer,* depth_buffer;
  GLuint * count,reference;
};

#endif /* FRAMEBUFFER_HPP_ */
