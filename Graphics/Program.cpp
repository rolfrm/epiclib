/*
 * Program.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: sebastian
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include "Program.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ShaderObject::ShaderObject(int _gl_ref):
  gl_ref(_gl_ref)
{
  
}

ShaderObject::ShaderObject(): gl_ref(-1){

}

int & ShaderObject::GetGLRef(){
  return gl_ref;
}

void ShaderObject::Dispose(){
  if(gl_ref != -1){
    glDeleteShader(gl_ref);
  }
}


GLuint shaderTypes[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER};

Shader Shader::FromFile(const char * shaderPath, ShaderType type){
	std::ifstream is (shaderPath,std::ios::in);
	std::stringstream buffer;
	buffer<<is.rdbuf();
	std::string shaderstring=buffer.str();
	return Shader(shaderstring.c_str(),type);
}

Shader::Shader(const char * shader_code,ShaderType type):glReference(ShaderObject(-1)){
 
  int reference = glCreateShader(shaderTypes[(int)type]);
  const char * s = shader_code;

  glShaderSource(reference, 1, &s, NULL);
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
    case ShaderType::Vertex: strShaderType = "vertex"; break;
    case ShaderType::Geometry: strShaderType = "geometry"; break;
    case ShaderType::Fragment: strShaderType = "fragment"; break;
    }
    
    std::cout<<"shader code "<<std::string(shader_code)<<"\n";
    
    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    delete[] strInfoLog;
  }

  glReference = reference;
  this->type = type;
}

void Program::init(Shader vertex_shader, Shader fragment_shader){
  reference=glCreateProgram();
  glAttachShader(reference,vertex_shader.glReference.Get().GetGLRef());
  glAttachShader(reference,fragment_shader.glReference.Get().GetGLRef());

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
Program::Program(Shader vertex_shader,Shader fragment_shader){
  init(vertex_shader,fragment_shader);
}

Program::Program(const char * path_vertex_shader,const char * path_fragment_shader){
  Shader vertex_shader = Shader::FromFile(path_vertex_shader,ShaderType::Vertex);
  Shader fragment_shader = Shader::FromFile(path_fragment_shader,ShaderType::Fragment);
  init(vertex_shader,fragment_shader);
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
void Program::BindAttribute(std::string name,GLuint location)
{
  glBindAttribLocation(reference,location,name.c_str());
};

void Program::Link(){glLinkProgram(reference);};
	

