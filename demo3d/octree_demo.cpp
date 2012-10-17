#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "../Math/GenericVector.hpp"
#include "../Math/Matrix.h"
#include "../Math/Quat.hpp"
#include "../Math/CommonMatrixOperations.hpp"
#include "Polygon.h"
#include <iostream>
#include <map>
Program * simpleShader;




int main(){
  
  Vec<float,4> iddata[4] = {vec(1.0f,0.0f,0.0f,0.0f),
			    vec(0.0f,1.0f,0.0f,0.0f),
			    vec(0.0f,0.0f,1.0f,-1.0f),
			    vec(0.0f,0.0f,0.0f,0.0f)};
  Vec<float,4> iddata2[4] = {vec(1.0f,0.0f,0.0f,0.0f),
			    vec(0.0f,1.0f,0.0f,0.0f),
			    vec(0.0f,0.0f,1.0f,0.0f),
			    vec(0.0f,0.0f,0.0f,1.0f)};

  Matrix<float, 4> identity(&iddata[0][0]);
  Matrix<float, 4> identity2(&iddata2[0][0]);
  
  float far = 10.0;
  float near = 0.1;

  double data [] = {
    0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 1.0, 0.0,  0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0,  0.0, 1.0, 1.0
    
  };
  
  initOpenGL(400,400);
  VertexBufferObject vbo(data,8,3,VBODrawType::STATIC);
  simpleShader = new Program(
			     Shader::FromFile("simple.vert",
					      ShaderType::VERTEX),
			     Shader::FromFile("simple.frag",
					      ShaderType::FRAGMENT));
  simpleShader->UseProgram();
  simpleShader->setUniformMat4x4("Projection",&iddata[0][0]);
  simpleShader->setUniformMat4x4("Modelview",&iddata[0][0]);
  simpleShader->setUniform("Color",0.0,1.0,1.0,1.0);
  simpleShader->UseProgram();
  Polygon p1;
  p1.Load(vbo,8,0);
  float it = 0.0;
  Matrix<float,4> smallR;

  GenerateObject2World(smallR.asPtr(),vec3(0,0,0),vec3(0.021,0.022,-0.023),vec3(1.0,1.0,1.0));
 
  Matrix<float,4> o(&iddata2[0][0]);
  Matrix<float,4> cam = Matrix<float,4>::Eye();
  while(true){
    it += 0.01;
    GenerateObject2World(cam.asPtr(),vec3(0,0,-it),vec3(it,0,0),vec3(1,1,1));
    o = smallR*o;
    
    Matrix<float,4> t =  Translate(o ,0,0,-5);
    t = cam * t;
    simpleShader->setUniformMat4x4("Modelview",t.asPtr());
    ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    p1.Draw(DrawMethod::QUADS);
    SwapBuffers();
    Sleep(0.01);
  }

}
