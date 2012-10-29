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

class Shader3D{
public:
  
  virtual void SetModelView(Matrix<float,4> modelView) = 0;
  virtual void SetProjection(Matrix<float,4> projection) = 0;
  virtual void SetCamera(Matrix<float,4> camera) = 0;
  virtual DrawMethod GetDrawMethod() = 0;
};

class FlatShader3D: public Shader3D{
public:
  Program shader;
  Matrix<float,4> projection;
  Matrix<float,4> camera;
  Matrix<float,4> total;
  Matrix<float,4> model;
  FlatShader3D():
    shader(Shader::FromFile("simple.vert", ShaderType::Vertex),
	   Shader::FromFile("simple.frag", ShaderType::Fragment))
  {
    projection = Matrix<float,4>::Eye();
    camera = projection;
    total = camera;
    model = total;
  }

  void update(){
    total = projection * camera * model;
    print(total);
    shader.setUniformMat4x4("T",total.AsPtr());
  }
  
  void SetModelView(Matrix<float,4> modelView){
    model = modelView;
    update();
  }
  
  void SetProjection(Matrix<float,4> _projection){
    projection = _projection;
    update();
  }

  void SetCamera(Matrix<float,4> cameraMat){
    camera = cameraMat;
    update();
  }
  DrawMethod GetDrawMethod(){
    return DrawMethod::Quads;
  }


};

class Tetragon{
  
public:
  Tetragon(Matrix<float,4> tr, Polygon _pgon, Texgon _tgon){
    TR = tr;
    pgon = _pgon;
    tgon = _tgon;
  }
  Matrix<float,4> TR;
  Polygon pgon;
  Texgon tgon;
  std::list<Tetragon> subGons;

  void AttachTetragon(Tetragon tgon){
    subGons.push_back(tgon);
  }

  void Draw(SharedPtr<Shader3D *> shader,Matrix<float,4> T){
    Matrix<float,4> relative = T*TR;
    
    shader.Get()->SetModelView(TR);
    tgon.Bind();
    pgon.Draw(shader.Get()->GetDrawMethod());
    for(auto it = subGons.begin(); it != subGons.end();it++){
      (*it).Draw(shader,relative);
    }
  }

};

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

typedef SharedPtr<GameObject *> GameObjectBase ;

class SimpleGuy: public GameObject{
public:
  DrawMethod dw2;
  void Draw(){
    
  }
};


void Render(Polygon polygon, Texgon texgon){

}


#include<list>
int main_test(){
  
  StopWatch swatch;
  swatch.Start();
  std::cout << "Elapsed: " << swatch.ElapsedSeconds() << "\n";
  Matrix<float,4> P = ProjectionMatrix(1,1,1.0,20000.0);
  
  Camera cam;
  std::list<SharedPtr<GameObject *> > golist;
  init_events();
  CameraControl ev;
  
  mouse_move_spawner.register_listener(&ev);
  key_event_handler.register_listener(&ev);
  simpleShader = new Program(
			     Shader::FromFile("simple.vert",
					      ShaderType::Vertex),
			     Shader::FromFile("simple.frag",
					      ShaderType::Fragment));
  simpleShader->UseProgram();
  simpleShader->setUniform("tex",0);
  
  double data [] = {
    -1.0, -1.0, -1.0,  1.0, -1.0, -1.0,  1.0, 1.0, -1.0,  -1.0, 1.0, -1.0,
    -1.0, -1.0, 1.0,  -1.0, 1.0, 1.0,  1.0, 1.0, 1.0,  1.0, -1.0, 1.0
  };
  
  double planeData [] = {-10000,0,-10000, 
			 -10000,0,10000,
			 10000,0,10000,
			 10000,0,-10000 };



  Polygon p1;
  p1.Load(VertexBufferObject(data,8,3,VBODrawType::Static),8,0);
  
  Polygon plane;
  plane.Load(VertexBufferObject(planeData,4,3,VBODrawType::Static),4,0);


  GameObject * g2 = new GameObject();
  g2->polygon = plane;
  Texture tex;
  tex = Texture::FromFile("grass.png",Interpolation::Linear,TextureWrap::Repeat,PixelFormat::RGB);
  g2->tgon.Load(tex,0);
  
  g2->pos = vec(0.0f,0.0f,0.0f);
  g2->rot = vec(0.0f,0.0f,0.0f);
  g2->drawMethod = DrawMethod::Quads;
  GameObjectBase gob = g2;
  golist.push_back(gob);
  float it = 0.0;
  StopWatch swatch2;
  g2->tgon.Bind();

  SharedPtr<Shader3D *> flat = new FlatShader3D();
  flat.Get()->SetProjection(P);
  Tetragon tetragon(Matrix<float,4>::Eye(),g2->polygon,g2->tgon);

  

  for(int i = 0; i < 100;i++){
    float x = (rand() % 1000) * 0.001 - 0.5;
    float y = (rand() % 1000) * 0.001 - 0.5;
    float z = (rand() % 1000) * 0.001 - 0.5;


    GameObject * go = new SimpleGuy();
    go->polygon = p1;
    go->rot = vec(0.0f,0.0f,0.0f);
    go->pos = vec(-x*50,-y*50,-z*50);
    go->drawMethod = DrawMethod::LineLoop;
    
    golist.push_back(go);
  }

  while(true){
    cam =  ev.GetCamera();
    swatch2.Reset();
    swatch2.Start();
    swatch.Reset();
    swatch.Start();
    ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    simpleShader->setUniform("Color",cos(it*2.1)* 0.5 + 0.5,cos(it*3.1 + 1)* 0.5 + 0.5,cos(it*3.1 + 3.0)*0.5 + 0.5 ,1.0);
    Vec<float,4> dir = vec(-(ev.left - ev.right),0.0f,ev.forward - ev.backward,1.0f); 
    it += 0.01;
    Matrix<float,4> c = cam.getTransformMatrix();
    flat.Get()->SetCamera(c);
    tetragon.Draw(flat,Matrix<float,4>::Eye());
    /*for(auto list_it = golist.begin(); list_it != golist.end(); list_it++){
      Vec<float,3> p = (**list_it)->pos;
      Vec<float,3> r = (**list_it)->rot;
      Matrix<float,4> TRS = TRSMatrix(p,r,vec(1.0f,1.0f,1.0f));
      
      Matrix<float,4> t =  P * c * TRS;
      simpleShader->setUniformMat4x4("T",t.AsPtr());
      if((**list_it)->drawMethod == DrawMethod::Quads){
	simpleShader->setUniform("Color",
				 cos(it*2.1) * 0.5 + 0.5,
				 sin(it*3.1 + 1)* 0.5 + 0.5,
				 sin(it*3.1 + 3.0)*0.5 + 0.5 ,
				 1.0);
	(**list_it)->polygon.Draw((**list_it)->drawMethod);
      }else{
	(**list_it)->polygon.Draw((**list_it)->drawMethod);
      }
      }*/
    SwapBuffers();
    Sleep(1/60.0 - swatch.ElapsedSeconds());
    std::cout << " " <<  1.0 / swatch2.ElapsedSeconds() << " fps\n";
  }
}

int main(){
  initOpenGL(1000,1000);
  
  main_test();
  std::cout << "Testing for  mem corruption\n";
  int * i = new int[12345];
  delete i;
  std::cout << "Out of scope out of mind..\n";
  return 0;

}
