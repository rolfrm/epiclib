/*
 * Program.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: sebastian
 */


#include "Program.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


Shader::Shader(const char * shader_path,GLenum type){
	std::ifstream is (shader_path,std::ios::in);
	std::stringstream buffer;
	buffer<<is.rdbuf();
	std::string shaderstring=buffer.str();


	reference = glCreateShader(type);
	const char * s =shaderstring.c_str();
	glShaderSource(reference,1,&s,NULL);
	glCompileShader(reference);


	GLint status;
	glGetShaderiv(reference, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE){
		GLint infoLogLength;
		glGetShaderiv(reference, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(reference, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
	    switch(type){
	        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
	        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
	        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
	    }
	    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
	    delete[] strInfoLog;
	}

	count = new GLuint;
	*count=1;
	this->type=type;

}


Shader::Shader(const Shader & original){
	reference=original.reference;
	type=original.type;
	count=original.count;
	*count+=1;
}

Shader::~Shader(){
	*count-=1;
	if(count==0){
		delete count;
		glDeleteShader(reference);
	}
}

Program::Program(Shader vertex_shader,Shader fragment_shader){
	reference=glCreateProgram();
	glAttachShader(reference,vertex_shader.reference);
	glAttachShader(reference,fragment_shader.reference);

    BindAttribute("pos",0);
    BindAttribute("uv",1);

	Link();
    GLint status;
    glGetProgramiv (reference, GL_LINK_STATUS, &status);
    if (status == GL_FALSE){
        GLint infoLogLength;
        glGetProgramiv(reference, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(reference, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    count=new GLuint;
    *count=1;
}

Program::Program(const char * path_vertex_shader,const char * path_fragment_shader){
	Shader vertex_shader(path_vertex_shader,GL_VERTEX_SHADER),fragment_shader(path_fragment_shader,GL_FRAGMENT_SHADER);
	reference=glCreateProgram();
	glAttachShader(reference,vertex_shader.reference);
	glAttachShader(reference,fragment_shader.reference);

    BindAttribute("pos",0);
    BindAttribute("uv",1);

	Link();
    GLint status;
    glGetProgramiv (reference, GL_LINK_STATUS, &status);
    if (status == GL_FALSE){
        GLint infoLogLength;
        glGetProgramiv(reference, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(reference, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    count=new GLuint;
    *count=1;
}

Program::Program(const Program & original){
	reference=original.reference;
	count=original.count;
	*count+=1;
}

Program::~Program(){
	*count-=1;
	if(count==0){
		delete count;
		glDeleteProgram(reference);
	}
}

GLuint Program::getUniformLocation(const char * name){
	return glGetUniformLocation(reference,name);
}

void Program::setUniform(const char * name,float f1){
	glUniform1f(getUniformLocation(name),f1);
}

void Program::setUniform(const char * name,float f1,float f2){
	glUniform2f(getUniformLocation(name),f1,f2);
}

void Program::setUniform(const char * name,float f1,float f2,float f3){
	glUniform3f(getUniformLocation(name),f1,f2,f3);
}

void Program::setUniform(const char * name,float f1,float f2,float f3,float f4){
	glUniform4f(getUniformLocation(name),f1,f2,f3,f4);
}

void Program::setUniform(const char * name,double d1){
	glUniform1f(getUniformLocation(name),d1);
}

void Program::setUniform(const char * name,double d1,double d2){
	glUniform2f(getUniformLocation(name),d1,d2);
}

void Program::setUniform(const char * name,double d1,double d2,double d3){
	glUniform3f(getUniformLocation(name),d1,d2,d3);
}

void Program::setUniform(const char * name,double d1,double d2,double d3,double d4){
	glUniform4f(getUniformLocation(name),d1,d2,d3,d4);
}

void Program::setUniform(const char * name,int i1){
	glUniform1i(getUniformLocation(name),i1);
}

void Program::setUniform(const char * name,int i1,int i2){
	glUniform2i(getUniformLocation(name),i1,i2);
}

void Program::setUniform(const char * name,int i1,int i2,int i3){
	glUniform3i(getUniformLocation(name),i1,i2,i3);
}

void Program::setUniform(const char * name,int i1,int i2,int i3,int i4){
	glUniform4i(getUniformLocation(name),i1,i2,i3,i4);
}

void Program::setUniformMat2x2(const char * name,float * mat){
	glUniformMatrix2fv(getUniformLocation(name),1,false,mat);
}

void Program::setUniformMat3x3(const char * name,float * mat){
	glUniformMatrix3fv(getUniformLocation(name),1,false,mat);
}

void Program::setUniformMat4x4(const char * name,float * mat){
	glUniformMatrix4fv(getUniformLocation(name),1,false,mat);
}

void Program::UseProgram(){
	glUseProgram(reference);
}

GLint Program::getCurrentProgram(){
	GLint current_program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&current_program);
	return current_program;
}


