#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "Polygon.h"
#include "Camera.h"
#include "../Math/CommonMatrixOperations.hpp"
#include "../Events/input_events.h"
#include <iostream>
#include <chrono>
#include <map>
Program * simpleShader;

class GameObject{
public:
  Polygon polygon;
  Vec<float,3> pos;
  Vec<float,3> rot;
  DrawMethod drawMethod;
};


class CameraControl: public EventListener<mouse_position>, public EventListener<KeyEvent>{
public:
  Vec<float,3> rot;
  Vec<float,3> pos2;
  float rx,ry,rz;
  mouse_position pos;
  bool first;
  float forward;
  float backward;
  float left;
  float right;
  

  CameraControl(){
    first = true;
    rx = 0;
    ry = 0; 
    rz = 0;
    pos.x = 0;
    pos.y = 0;
    forward = 0.0;
    backward = 0.0;
  }

  Camera GetCamera(){

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
using namespace std::chrono;
#include<list>
int main(){
  initOpenGL(400,400);
  
  high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
  Sleep(0.99);
  high_resolution_clock::time_point tp2 = std::chrono::high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(tp2 - tp);
  std::cout << "Time: " << time_span.count() << "\n";
  

  return 0;


  Matrix<float,4> P = ProjectionMatrix(2,2,5.0,20000.0);
  Camera cam;
  std::list<GameObject *> golist;

  init_events();
  CameraControl ev;
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

  double planeData [] = {-1000,0,-1000, 
		     -1000,0,1000,
		     1000,0,1000,
		     1000,0,-1000};


  Polygon p1;
  VertexBufferObject vbo(data,8,3,VBODrawType::STATIC);
  p1.Load(vbo,8,0);
  VertexBufferObject pvbo = VertexBufferObject(planeData,4,3,VBODrawType::STATIC);
  Polygon plane;
  plane.Load(pvbo,4,0);
  
  for(int i = 0; i < 1000;i++){
    float x = (float) ( i % 2);
    float y = (float) ( i / 2);
    float z = (float) ( i / 2);
    GameObject * go = new GameObject();
    go->polygon = p1;
    go->rot = vec(0.0f,0.0f,0.0f);
    go->pos = vec(x*5,y*5,z*5);
    go->drawMethod = DrawMethod::LINE_LOOP;
    golist.push_back(go);
  }

  GameObject g2 = {plane,vec(0.0f,0.0f,0.0f),vec(0.0f,0.0f,0.0f),DrawMethod::QUADS};
  golist.push_back(&g2);
  float it = 0.0;

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
    dir = cr * dir;
    
    for(auto list_it = golist.begin(); list_it != golist.end(); list_it++){
      Vec<float,3> p = (*list_it)->pos;
      Vec<float,3> r = (*list_it)->rot;
      Matrix<float,4> TRS = TRSMatrix(p,r,vec(1.0f,1.0f,1.0f));
      
      Matrix<float,4> t =  P * c * TRS;
      simpleShader->setUniformMat4x4("T",t.asPtr());
      if((*list_it)->drawMethod == DrawMethod::QUADS){
	simpleShader->setUniform("Color",cos(it*2.1) * 0.5 + 0.5,sin(it*3.1 + 1)* 0.5 + 0.5,sin(it*3.1 + 3.0)*0.5 + 0.5 ,1.0);

	(*list_it)->polygon.Draw((*list_it)->drawMethod);
      }else{
	(*list_it)->polygon.Draw((*list_it)->drawMethod);
      }
    }

    
    

    SwapBuffers();
    Sleep(0.01);
    cam = cam.move(vec(dir[0],dir[1],dir[2]));
  }

}
