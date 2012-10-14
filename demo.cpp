#include <GL/glew.h>
#include "Graphics/GraphicsCore.hpp"
#include "Graphics/Program.hpp"
#include "Graphics/VertexBufferObject.hpp"
#include "Events/input_events.h"


class Listener1:EventListener<KeyEvent>{

  bool HandleEvent(KeyEvent kev){

  }
};



int main(){
  
  float data[] = {0, 0, 1,0 , 1,1, 0,1};
  initOpenGL(800,600);
  init_events();
  VertexBufferObject vbo(data,8,2);
  Shader vert = Shader::FromFile("vert_naive.vert",ShaderType::VERTEX);
  Shader frag = Shader::FromFile("frag_naive.frag",ShaderType::FRAGMENT);
  Program p1(vert,frag);
  p1.UseProgram();

	while(UpdateGraphics()){
	  vbo.BindBuffer(0);
	  glDrawArrays(GL_LINE_LOOP,0,4);
	}
}
