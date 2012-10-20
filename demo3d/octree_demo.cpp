#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "Polygon.h"
#include "Camera.h"
#include "../Math/CommonMatrixOperations.hpp"
#include <iostream>
#include <map>
Program * simpleShader;

int main(){
  Matrix<float,4> P = ProjectionMatrix(2,2,4.0,1000.0);
  Camera cam;
  initOpenGL(400,400);
  simpleShader = new Program(
			     Shader::FromFile("simple.vert",
					      ShaderType::VERTEX),
			     Shader::FromFile("simple.frag",
					      ShaderType::FRAGMENT));
  simpleShader->UseProgram();
  simpleShader->setUniformMat4x4("Projection",P.asPtr());
  
  simpleShader->UseProgram();

  double data [] = {
    0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 1.0, 0.0,  0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0,  0.0, 1.0, 1.0
    
  };

  Polygon p1;
  VertexBufferObject vbo(data,8,3,VBODrawType::STATIC);
  p1.Load(vbo,8,0);

  float it = 0.0;
  
  while(true){
    simpleShader->setUniform("Color",cos(it*3.1)* 0.5 + 0.5,cos(it*3.1 + 1.5)* 0.5 + 0.5,cos(it*3.1 + 3.0)*0.5 + 0.5 ,1.0);
    cam = cam.rotate(vec(0.01f,0.01f,0.0f));
    //cam = cam.move(vec(0.0f,0.0f,0.01f));
    it += 0.01;
    Matrix<float,4> c = cam.getTransformMatrix();
    
    
    Matrix<float,4> t =  TRSMatrix(vec(0.0f,0.0f,-1.0f*it - 5.0f),
				   vec(0.0f,0.0f,(float)it*15.3f),
				   vec(1.0f,1.0f,1.0f));
    t = c * t;
    simpleShader->setUniformMat4x4("Modelview",t.asPtr());
    //ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    p1.Draw(DrawMethod::LINE_LOOP);
    SwapBuffers();
    Sleep(0.005);
  }

}
