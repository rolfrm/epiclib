/*
 * Program.hpp
 *
 *  Created on: Aug 2, 2012
 *      Author: sebastian
 */

#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

#include <string>
#include "../Math/GenericVector.h"
#include "../Utils/SharedPtr.h"

enum class ShaderType: int{Vertex = 0, Fragment = 1, Geometry = 2};

class ShaderObject{
  int gl_ref;
public:
  ShaderObject(int gl_ref);
  ShaderObject();
  int & GetGLRef();
  void Dispose();
};

class Shader{
public:
  SharedPtr<ShaderObject> glReference;
  Shader(const char * shader_str,ShaderType type);
  static Shader FromFile(const char * shaderPath, ShaderType shaderType);
  ShaderType type;
};


class Program{
  void init(Shader vertexShader, Shader fragmentShader);
  
public:
	Program(){
		count = new unsigned int;
		*count=1;
		reference=0;
	};
	Program(Shader vertex_shader,Shader fragment_shader);
	Program(const char * path_vertex_shader,const char * path_fragment_shader);
	Program(const Program & original);
	~Program();

	int getCurrentProgram();
	unsigned int getUniformLocation(const char * name);

	void setUniform(const char * name,float f1);
	void setUniform(const char * name,float f1,float f2);
	void setUniform(const char * name,float f1,float f2,float f3);
	void setUniform(const char * name,float f1,float f2,float f3,float f4);

	void setUniform(const char * name,double d1);
	void setUniform(const char * name,double d1,double d2);
	void setUniform(const char * name,double d1,double d2,double d3);
	void setUniform(const char * name,double d1,double d2,double d3,double d4);

	void setUniform(const char * name,int i1);
	void setUniform(const char * name,int i1,int i2);
	void setUniform(const char * name,int i1,int i2,int i3);
	void setUniform(const char * name,int i1,int i2,int i3,int i4);

  template<class T> 
  void setUniform(const char * name, Vec<T,1> v){\
    setUniform(name,v.data[0]);
  }
  template<class T> 
  void setUniform(const char * name, Vec<T,2> v){\
    setUniform(name,v.data[0],v.data[1]);
  }
  template<class T> 
  void setUniform(const char * name, Vec<T,3> v){\
    setUniform(name,v.data[0],v.data[1],v.data[2]);
  }
  template<class T> 
  void setUniform(const char * name, Vec<T,4> v){\
    setUniform(name,v.data[0],v.data[1],v.data[2],v.data[3]);
  }
  
  void setUniformMat2x2(const char * name,float * mat);
  void setUniformMat3x3(const char * name,float * mat);
  void setUniformMat4x4(const char * name,float * mat);

  void BindAttribute(std::string name,unsigned int location);
  void UseProgram();
  void Link();
  unsigned int * count,reference;
};




#endif /* PROGRAM_HPP_ */
