#pragma once
#include "epiclib3/Graphics/Program.hpp"
#include "Color.h"
class Texture2DShader{

  Program shader;
public:
  Texture2DShader(char * vert, char * frag):shader(Shader(vert,ShaderType::Vertex),
			   Shader(frag,ShaderType::Fragment)){

  }

  void SetPos(double x, double y){
    shader.UseProgram();
    shader.setUniform("position",x,y);
  }

  void SetSize(double w, double h){
    shader.UseProgram();
    shader.setUniform("size",w,h);
  }

  void SetColor(Color col){
    shader.UseProgram();
    shader.setUniform("color",col.As<float>() / 255.0f);
  }

  void SetTexture(unsigned int channel){
    shader.UseProgram();
    shader.setUniform("tex",(int)channel);
  }
  void SetUVState(Vec<double,2> pos, Vec<double,2> size){
    shader.UseProgram();
    shader.setUniform("uvSize",size);
    shader.setUniform("uvPos",pos);
  }
};
