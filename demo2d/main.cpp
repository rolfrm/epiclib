#include "QuadtreeRenderer.h"
#include "infinidraw_interface.h"
#include<fstream>
#include "SimpleEvents.h"
#include "epiclib3/Graphics/Framebuffer.hpp"
#include "epiclib3/Graphics/GraphicsCore.hpp"
#include<thread>
#include<GL/glfw.h>
using namespace std;

void Sleep(double t){
  glfwSleep(t);
}
int test_main(){
  fstream fstr;
  fstr.open("test.save", ios::in);
  /*QuadTree * qt;
  if(!fstr.good()){
    qt = genQuadTree();
  }else{
    qt = loadQuadtreeState(fstr);
    }*/
  fstr.close();
  
  float squaredata[] = {0,0,
			1.0,0.0,
			1.0,1.0,
			0.0,1.0};
  VBO SquareBuffer(squaredata,4,2,VBODrawType::Static);
  SimpleEvents sev(QuadTreeRenderer,*texShader,SquareBuffer);
  key_event_handler.register_listener(&sev);
  mouse_click_handler.register_listener(&sev);
  mouse_move_spawner.register_listener(&sev);
  mouse_wheel_event_spawner.register_listener(&sev);
  window_resize_event.register_listener(&sev);
  
  while(sev.Running){
    FrameBuffer::screenBuffer.Clear();
    sev.RenderBrushPreview();
    infinidraw_render();
    Sleep(0.01);
    SwapBuffers();
  }
  fstr.open("test.save", ios::out | ios::trunc);
  writeQuadTreeState(QuadTreeRenderer->origin,fstr);
  fstr.close();
  std::cout << "Ending\n";
  return 0;
}
void uimain();
int main(){
  initOpenGL(512,512);
  init_events();
  init_infinidraw();
  std::thread t(uimain);
  

  return test_main();
}
