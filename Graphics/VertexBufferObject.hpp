/*
 * VertexBufferObject.hpp
 *
 *  Created on: Aug 4, 2012
 *      Author: sebastian
 */

#ifndef VERTEXBUFFEROBJECT_HPP_
#define VERTEXBUFFEROBJECT_HPP_

#include <GL/glew.h>
#include <GL/gl.h>


class VertexBufferObject{
public:
	VertexBufferObject(GLfloat * data,GLuint vertex_count,GLuint vertex_dimension,GLenum buffer_param=GL_STATIC_DRAW);
	VertexBufferObject(const VertexBufferObject & orginal);
	~VertexBufferObject();

	void BindBuffer(GLuint index=0);

	GLuint * count,reference,n_vertex,dim_vertex;
};




#endif /* VERTEXBUFFEROBJECT_HPP_ */
