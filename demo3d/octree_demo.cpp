#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "Polygon.h"
#include "Camera.h"
#include <iostream>
#include <map>
Program * simpleShader;

Matrix<float,4> GetObjectTransformation(Vec<double,3> pos, Vec<double,3> rot){
  Matrix<float,4> o;
  GenerateObject2World(o.asPtr(),
		       vec3(pos[0],pos[1],pos[2]),
		       vec3(rot[0],rot[1],rot[2]),
		       vec3(1,1,1));
  return o;
}

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
  
  Camera cam;
  

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
  while(true){
    cam = cam.rotate(vec(0.0,0.0,0.0));
    cam = cam.move(vec(0.0,0.0,-0.05));
    it += 0.01;
    Matrix<float,4> c = cam.getTransformMatrix();
    c.print();
    o = smallR*o;
    
    Matrix<float,4> t =  Translate(o ,0,0,-2);
    t = c * t;
    simpleShader->setUniformMat4x4("Modelview",t.asPtr());
    ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    p1.Draw(DrawMethod::QUADS);
    SwapBuffers();
    Sleep(0.01);
  }

}
