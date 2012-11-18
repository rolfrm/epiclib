#include "Shader.h"
FlatShader3D::FlatShader3D():
shader(Shader::FromFile("simple.vert", ShaderType::Vertex),
		   Shader::FromFile("simple.frag", ShaderType::Fragment))
{
  projection = Matrix<float,4>::Eye();
  camera = projection;
  total = camera;
  model = total;
  shader.UseProgram();
  shader.setUniform("tex",0);
}

void FlatShader3D::update(){
  shader.UseProgram();
  total = projection * camera * model;
  shader.setUniformMat4x4("T", total.AsPtr());
}
  
void FlatShader3D::SetModelView(Matrix<float,4> modelView){
  model = modelView;
  update();
}
  
void FlatShader3D::SetProjection(Matrix<float,4> _projection){
  projection = _projection;
  update();
}

void  FlatShader3D::SetCamera(Matrix<float,4> cameraMat){
  camera = cameraMat;
  update();
}
DrawMethod FlatShader3D::GetDrawMethod(){
  return DrawMethod::Quads;
}
