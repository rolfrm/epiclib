#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "Polygon.h"
#include "Camera.h"
#include "../Math/CommonMatrixOperations.hpp"
#include "../Events/input_events.h"
#include <iostream>

#include <map>
Program * simpleShader;

class GameObject{

  Polygon polygon;
  Vec<float,3> pos;
  Vec<float,3> rot;

};


class eventHandler: public EventListener<mouse_position>, public EventListener<KeyEvent>{
public:
  float rx,ry,rz;
  mouse_position pos;
  bool first;
  float forward;
  float backward;
  float left;
  float right;
  eventHandler(){
    first = true;
    rx = 0;
    ry = 0; 
    rz = 0;
    pos.x = 0;
    pos.y = 0;
    forward = 0.0;
    backward = 0.0;
  }
  bool handle_event(mouse_position npos){
    
    int dx = npos.x - 200;
    
    int dy = npos.y - 200;
    
    SetMousePosition(200,200);
    
    rx += (float) dy / 1000.0;
    ry += (float) dx / 1000.0;
    std::cout << rx << " " << ry << "\n";
    return true;
  }
  bool handle_event(KeyEvent ke){
    switch(ke.key){
    case ARROW_UP:forward = (ke.pressed ? 1 : 0); break;
    case ARROW_DOWN:backward = (ke.pressed ? 1 : 0); break;
    case ARROW_LEFT:left = (ke.pressed ? 1 : 0); break;
    case ARROW_RIGHT:right = (ke.pressed ? 1 : 0); break;
    }
    return true;
  }

};

class cameraListener: public EventListener<Camera>{
public:
  
  bool handle_event(Camera cam){
    std::cout << "camera listener\n";
    return true;
  }

};

template<class T,int size>
void print(Matrix<T,size> mat){
  for(int row = 0; row < size; row++){
    std::cout << " |";
    for(int col = 0; col < size; col++){
      std::cout << mat[col][row] << " ";
    }
    std::cout << "|\n";
  }
}

template<class T, int size>
void print(Vec<T,size> vec){
  std::cout << "(";
  for(int i = 0; i < size; i++){
    std::cout << vec[i] << " ";
  }
  std::cout << ")\n";

}


int main(){
  
  Matrix<float,4> P = ProjectionMatrix(2,2,1.0,1000.0);
  Camera cam;
  EventSpawner<Camera> camSpawn;
  cameraListener cl;
  camSpawn.register_listener(&cl);

  initOpenGL(400,400);
  init_events();
  eventHandler ev;
  mouse_move_spawner.register_listener(&ev);
  key_event_handler.register_listener(&ev);
  simpleShader = new Program(
			     Shader::FromFile("simple.vert",
					      ShaderType::VERTEX),
			     Shader::FromFile("simple.frag",
					      ShaderType::FRAGMENT));
  simpleShader->UseProgram();
 
  double data [] = {
    0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 1.0, 0.0,  0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0,  1.0, 0.0, 1.0
    
  };

  Polygon p1;
  VertexBufferObject vbo(data,8,3,VBODrawType::STATIC);
  p1.Load(vbo,8,0);

  float it = 0.0;
  Vec<float,4> tester = vec(1.0f,1.0f,1.0f,1.0f);
  
  while(true){
    ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    simpleShader->setUniform("Color",cos(it*2.1)* 0.5 + 0.5,cos(it*3.1 + 1)* 0.5 + 0.5,cos(it*3.1 + 3.0)*0.5 + 0.5 ,1.0);
    Vec<float,4> dir = vec(-(ev.left - ev.right),0.0f,ev.forward - ev.backward,1.0f); 
    cam = cam.SetRotation(vec(-ev.rx,-ev.ry,0.0f));
    it += 0.01;
    Matrix<float,4> c = cam.getTransformMatrix();
    Matrix<float,4> cr = c;
    cr[3][0] = 0;
    cr[3][1] = 0;
    cr[3][2] = 0;
    Matrix<float,4> t =  TRSMatrix(vec(0.0f,0.0f,-10.0f),
				   vec(0.0f,0.0f,it),
				   vec(1.0f,1.0f,1.0f));
    dir = cr * dir;
      

    t = c * t;
    std::cout << "-\n";
    print(dir);
    simpleShader->setUniformMat4x4("T",(P * t).asPtr());
    p1.Draw(DrawMethod::LINE_LOOP);
    SwapBuffers();
    Sleep(0.01);
    cam = cam.move(vec(dir[0],dir[1],dir[2]));
  }

}
