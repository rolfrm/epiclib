#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "../Utils/Debug.h"
#include "Polygon.h"
#include "Camera.h"
#include "../Math/CommonMatrixOperations.hpp"
#include "../Events/input_events.h"
#include <iostream>
#include <chrono>
#include <map>
#include "CameraControl.h"
#include "../Utils/SharedPtr.h"
Program * simpleShader;




class GameObject{
public:
  Polygon polygon;
  Texgon tgon;
  Vec<float,3> pos;
  Vec<float,3> rot;
  DrawMethod drawMethod;
  virtual void Draw(){
    
  }
};

class SimpleGuy: public GameObject{
public:
  DrawMethod dw2;
  void Draw(){
    
  }
};

class RefcountedGameObj:SharedPtr<GameObject>{
  
};

void Render(Polygon polygon, Texgon texgon){

}

void testSptr(){

   SharedPtr<double *> d(new double[255]);
   SharedPtr<double *> d2 = d;
   for(int i = 0; i < 900000000;i++){
     SharedPtr<double *> d3 = new double[256];
     (*d3)[0] = 1.0;
     
     d2 = d3;
     d2 = d;
     WeakRef<double *> d4 = d3.AsWeakRef();
     std::cout << (*d4)[0] << "\n";
   }
}

#include<list>
int main(){
  StopWatch swatch;
  swatch.Start();
  initOpenGL(1000,1000);
  std::cout << "Elapsed: " << swatch.ElapsedSeconds() << "\n";
  Matrix<float,4> P = ProjectionMatrix(1,1,1.0,20000.0);
  Camera cam;
  std::list<SharedPtr<GameObject *>> golist;

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
  simpleShader->setUniform("tex",0);
  double data [] = {
    -1.0, -1.0, -1.0,  1.0, -1.0, -1.0,  1.0, 1.0, -1.0,  -1.0, 1.0, -1.0,
    -1.0, -1.0, 1.0,  -1.0, 1.0, 1.0,  1.0, 1.0, 1.0,  1.0, -1.0, 1.0
  };

  double planeData [] = {-10000,0,-10000, 
		     -10000,0,10000,
		     10000,0,10000,
		     10000,0,-10000};
  Polygon p1;
  VertexBufferObject vbo(data,8,3,VBODrawType::STATIC);
  p1.Load(vbo,8,0);
  VertexBufferObject pvbo = VertexBufferObject(planeData,4,3,VBODrawType::STATIC);
  Polygon plane;
  
    GameObject * g2 = new GameObject();
    g2->polygon = plane;
    Texture2D tex = Texture2D("grass.png",GL_LINEAR,GL_REPEAT);
			      g2->tgon.Load(tex,0);
    
    g2->pos = vec(0.0f,0.0f,0.0f);
    g2->rot = vec(0.0f,0.0f,0.0f);
    g2->drawMethod = DrawMethod::QUADS;
    golist.push_back(g2);
    float it = 0.0;
    StopWatch swatch2;
    g2->tgon.Bind();
    
    while(true){
      for(int i = 0; i < 100;i++){
	float x = (rand() % 1000) * 0.001 - 0.5;
      float y = (rand() % 1000) * 0.001 - 0.5;
      float z = (rand() % 1000) * 0.001 - 0.5;


    GameObject * go = new SimpleGuy();
    go->polygon = p1;
    go->rot = vec(0.0f,0.0f,0.0f);
    go->pos = vec(-x*50,-y*50,-z*50);
    go->drawMethod = DrawMethod::LINE_LOOP;
    
      golist.push_back(go);
    }
    while(golist.size() > 600){
    
      golist.pop_front();
    
    }
      
    
    cam =  ev.GetCamera();
    
    swatch2.Reset();
    swatch2.Start();
    swatch.Reset();
    swatch.Start();
    glClear(GL_DEPTH_BUFFER_BIT);
    //ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    simpleShader->setUniform("Color",cos(it*2.1)* 0.5 + 0.5,cos(it*3.1 + 1)* 0.5 + 0.5,cos(it*3.1 + 3.0)*0.5 + 0.5 ,1.0);

    Vec<float,4> dir = vec(-(ev.left - ev.right),0.0f,ev.forward - ev.backward,1.0f); 
    
    it += 0.01;
    Matrix<float,4> c = cam.getTransformMatrix();
    Matrix<float,4> cr = c;
    cr[3][0] = 0;
    cr[3][1] = 0;
    cr[3][2] = 0;
    dir = cr * dir * 10;
    print(dir);
    for(auto list_it = golist.begin(); list_it != golist.end(); list_it++){
      Vec<float,3> p = (**list_it)->pos;
      Vec<float,3> r = (**list_it)->rot;
      Matrix<float,4> TRS = TRSMatrix(p,r,vec(1.0f,1.0f,1.0f));
      
      Matrix<float,4> t =  P * c * TRS;
      simpleShader->setUniformMat4x4("T",t.asPtr());
      if((**list_it)->drawMethod == DrawMethod::QUADS){
	simpleShader->setUniform("Color",cos(it*2.1) * 0.5 + 0.5,sin(it*3.1 + 1)* 0.5 + 0.5,sin(it*3.1 + 3.0)*0.5 + 0.5 ,1.0);

	(**list_it)->polygon.Draw((**list_it)->drawMethod);
      }else{
	(**list_it)->polygon.Draw((**list_it)->drawMethod);
      }
    }

    
    

    SwapBuffers();
    
    Sleep(1/60.0 - swatch.ElapsedSeconds());
    std::cout << " " <<  1.0 / swatch2.ElapsedSeconds() << " fps\n";
    
  }

}
