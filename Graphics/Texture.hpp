/*
 * Texture.hpp
 *
 *  Created on: Jul 28, 2012
 *      Author: sebastian
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <GL/gl.h>
#include <string>


class Texture2D{
public:
	Texture2D(std::string path,GLint interpolation=GL_NEAREST,GLint wrap=GL_CLAMP_TO_EDGE);
	Texture2D(GLuint width,GLuint height,GLuint texture_format,GLuint copies=1,
			GLuint interpolation=GL_NEAREST,GLuint wrap=GL_CLAMP_TO_EDGE,
			GLuint data_type=GL_UNSIGNED_BYTE,void * data = 0,GLuint data_format=GL_RGBA);
	Texture2D(const Texture2D & original);
	~Texture2D();

	void BindTexture(GLuint channel=0,GLuint texture=0);
	void UnbindTexture(GLuint channel=0);
	void Write2Texture(GLuint width,GLuint height,GLuint x,GLuint y,GLuint format,GLuint type,void * data);

	GLuint * reference,width,height,* count,n_tex;

};



#endif /* TEXTURE_HPP_ */
