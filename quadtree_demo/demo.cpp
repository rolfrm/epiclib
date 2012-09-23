#include "Graphics/GraphicsCore.hpp"
#include "Graphics/Program.hpp"
#include "Graphics/VertexBufferObject.hpp"
#include "Events/input_events.h"
#include "quadtree.h"
#include <list>

class ShaderClass{
  static Program * currentProgram;
  
public:
  Program prog;
  ShaderClass(Program _prog):prog(_prog){
   
  }

  void Bind(){
    if(!(currentProgram == &prog)){
      currentProgram = &prog;
      prog.UseProgram();
    }
  }
  Program & getProgram(){
    return prog;
  }
};
Program * ShaderClass::currentProgram;

class NaiveShader:public ShaderClass{
public:
  NaiveShader():
    ShaderClass(Program(
			Shader::FromFile("vert_naive.vert",GL_VERTEX_SHADER),
			Shader::FromFile("frag_naive.frag",GL_FRAGMENT_SHADER)))
  {
    
  }

  void pos(float x, float y){
    Bind();
    prog.setUniform("pos",x,y);
  }
  void scale(float x, float y){
    Bind();
    prog.setUniform("scale",x,y);

  }

  void cameraPosition(float x, float y){
    Bind();
    prog.setUniform("cameraPosition",x,y);
  }

  void cameraScale(float x, float y){
    Bind();
    prog.setUniform("cameraScale",x,y);
  }

  void color(float r, float g, float b, float a){
    Bind();
    prog.setUniform("Color",r,g,b,a);
  }

};

NaiveShader * naiveShader;

void QuadTreeDemo();
class Listener1:public EventListener<KeyEvent>, public EventListener<mouse_position>, public EventListener<MouseClick>{

  bool HandleEvent(KeyEvent kev){

  }
};

class Drawable{
public:
  
  double x, y, w, h;
  virtual void draw(){
    naiveShader->pos(x,y);
    naiveShader->scale(w,h);
    naiveShader->color(1,0,1,1);
    glDrawArrays(GL_QUADS,0,4);
  }
};

struct rect{
  double x,y,w,h;
};
struct rect1{
  double x,w;
};
#define max(X,Y) (X > Y ? X : Y)
rect1 Intersect(rect1 d1, rect1 d2){
  
  if(d1.x <= d2.x){
    return {d2.x,max(d1.x + d1.w - d2.x, d2.w)};
  }
    return Intersect(d2,d1);
}

rect Intersect(rect r1, rect r2){
  rect1 x = Intersect(rect1({r1.x,r1.w}),rect1({r2.x,r2.w}));
  rect1 y = Intersect(rect1({r1.y,r1.h}),rect1({r2.y,r2.h}));
  return rect({x.x,y.x,x.w,y.w});
}

typedef Node<std::list<Drawable *>,2> DrawNode;

#include <math.h>
#include <iostream>

class World{
public:
  
  DrawNode * origin;

  rect drawableToRect(Drawable * dw){
    return rect({dw->x,dw->y,dw->w,dw->h});
  }

  void ptRect(rect b){
    std::cout << b.x << " " << b.y << " " << b.w << " " << b.h << "\n"; 
  }

  World(){
    origin = new DrawNode();
  }
  
  DrawNode * GetNodeForRect(rect & a){
    DrawNode * relativeOrigin = origin;
    std::cout << "start ";
    ptRect(a);
    while((a.w>a.h ? a.w : a.h) >= 1){
      std::cout << "Zoom out.. "; ptRect(a);
      relativeOrigin = GoUp(a,relativeOrigin);
    }
    int mx = a.x;
    int my = a.y;
    relativeOrigin = relativeOrigin->relative_node(vec2(mx,my),true);
    a.x -= (double)mx;
    a.y -= (double)my;
    std::cout << "move ";
    ptRect(a);
    
    while((a.h > a.w ? a.h : a.w) <= 0.5){
      a.h *= 2;
      a.w *= 2;
      a.x *= 2;
      a.y *= 2;
      std::cout << "zoom in 1 :";
      ptRect(a);
      relativeOrigin = relativeOrigin->get_child(vec2(a.x,a.y),true);
      int mx = relativeOrigin->idx & 1;
      int my = relativeOrigin->idx >>1;
      a.x -= mx;
      a.y -= my;
      std::cout << "zoom in 2 :";
      ptRect(a);
    }
    return relativeOrigin;
  }

  void AddObject(Drawable * drawable){
    rect a = drawableToRect(drawable);
    DrawNode * relativeOrigin = GetNodeForRect(a);
    drawable->x = a.x;
    drawable->y = a.y;
    drawable->w = a.w;
    drawable->h = a.h;
    relativeOrigin->Data.push_back(drawable);
  }
  DrawNode * GoUp(rect & a, DrawNode * node){
    int x = node->idx & 1;
    int y = node->idx >> 1;
    std::cout << x << " " << y << " " << node->idx <<"\n";
    a.w = a.w / 2.0;
    a.h = a.h / 2.0;
    a.x = (a.x +(double) x) / 2.0;
    a.y = (a.y +(double) y) / 2.0;
    return node->get_parent(true);
  }
  void RenderFromOrigin(double x, double y, double w, double h){
    rect b = {x,y,w,h};
    std::cout << "Start renfer from origin " << x << " " << y << " " << w << " " << h << "\n";
    DrawNode * relativeOrigin = GetNodeForRect(b);
    //test: render down
    naiveShader->cameraPosition(b.x,b.y);
    naiveShader->cameraScale(b.w,b.h);
    std::cout << "Render start.. " << relativeOrigin << "\n";

    std::cout << relativeOrigin->parent << " ";
    for(int i = 0; i < 4; i++){
      std::cout << " " << relativeOrigin->children[i];
    }
    std::cout << "\n--";
    ptRect(b);
    std::cout << "\n";
    RenderDown(relativeOrigin, {0,0,1,1},rect({-b.x,-b.y,b.w/2,b.h/2}));
  }

  void RenderDown(DrawNode * nd, rect cameradef, rect camera){
    //std::cout << "camera def ";
    //ptRect(cameradef);
    //std::cout << "camera ... ";
    //ptRect(camera);
    if(nd->Data.size() > 0){
      naiveShader->cameraPosition(camera.x,camera.y);
      naiveShader->cameraScale(camera.w,camera.h);
      std::cout << camera.x << " " << camera.y << "\n";
      for(auto it = nd->Data.begin(); it != nd->Data.end(); it++){
	//std::cout << "Found object..\n";
	(*it)->draw();
      }
    }
    DrawNode * nd2;
    
    for(int x = 0; x < 2;x++){
      for(int y = 0; y < 2;y++){
	nd2 = nd->get_child(vec2(x,y),false); 
	//std::cout <<x << " " << y << " " <<  nd2 << "\n";
	if(nd2 != NULL){					
	  rect subC = Intersect(cameradef,rect({(double)x,(double)y,0.5,0.5}));	
	  if(true ||subC.w > 0 && subC.h > 0){				
	    subC.x *= 2; 
	    subC.y *= 2; 
	    subC.w *= 2; 
	    subC.h *= 2; 
	    RenderDown(nd2,subC,{(2.0*camera.x +(double)x),(2.0*camera.y +(double) y),camera.w*2.0,camera.h*2.0});
	    //std::cout << ",\n";
	    }else{
	    //std::cout << ".\n";
	    }
	}
      }
    }
  }
  
  void Print(DrawNode * node=NULL, DrawNode * prevnode = NULL){
    if(node == NULL){
      node = origin;
    }
    std::cout << "objects: " << node->Data.size() << "\n";
    for(int i= 0;i < 4;i++){
      DrawNode * nnode = node->get_child(i,false);

      if(nnode != NULL && nnode != prevnode){
	std::cout << "Going down.." << i << " " << node << "\n";
	Print(nnode,node);
      }
    }
    if(node->get_parent(false) != NULL && node->get_parent(false) != prevnode){
      std::cout << "Going up.. \n";
      Print(node->get_parent(false),node);
    }
    std::cout << "End of line\n";
  }

};


int main(){
  
  DrawNode dn;
  World world;

  QuadTreeDemo();
  
  float data[] = {-0.5, -0.5, 0.5,-0.5 , 0.5,0.5, -0.5,0.5};
  initOpenGL(800,600,30);
  glClearColor(0,0,0,0);
  init_events();
  VertexBufferObject vbo(data,8,2);
  naiveShader = new NaiveShader();
  naiveShader->cameraScale(1,1);
  naiveShader->cameraPosition(0,0);
  for(double i = 1; i < 10;i++){
    Drawable* dw = new Drawable();
    dw->x = 0.3*(i - 1.5);
    dw->y = 0.3*(i - 1.5);
    dw->w = 0.4/i;
    dw->h = 0.4/i;
    world.AddObject(dw);
  }
  world.Print();
  NaiveShader ns;
  ns.pos(-0.5,-0.5);
  ns.scale(1,1);
  ns.cameraScale(2,2);
  ns.cameraPosition(0,0);
  float t = -1.0;
  vbo.BindBuffer(0);
  while(UpdateGraphics()){
    t += 0.01;
    world.RenderFromOrigin(0,0,2.0+t,2.0+t);
  }
}
