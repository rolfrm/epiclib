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
#include<math.h>
NaiveShader * naiveShader;

void QuadTreeDemo();

class Listener1: public EventListener<KeyEvent>, public EventListener<mouse_position>, public EventListener<MouseClick>, public EventListener<MouseWheelEvent>,public EventListener<size>{
public:
  
  double scale;
  double x;
  double y;
  bool leftIsDown;
  double lastX;
  double lastY;
  int viewport_width;
  int viewport_height;
  Listener1(int _viewport_width, int _viewport_height){
    scale = 1.0;
    x = 0;
    y = 0;
    leftIsDown;
    viewport_width = _viewport_width;
    viewport_height = _viewport_height;
  }

  bool handle_event(size s){
    viewport_width = s.x;
    viewport_height = s.y;
    std::cout << "Window resize\n";
    return true;
  }
  bool handle_event(KeyEvent kev){
    return true;
  }

  bool handle_event(mouse_position mpos){
    if(leftIsDown){
    double nx = mpos.x;
    double ny = mpos.y;
    double dx = nx - lastX;
    double dy = ny - lastY;
    x -= dx*scale*0.01;
    y += dy*scale*0.01;
    lastX = nx;
    lastY = ny;
    }
    return true;
  }

  bool handle_event(MouseClick mclick){
    if(mclick.button == 0){
      leftIsDown = mclick.pressed;
      lastX = get_mouse_position().x;
      lastY = get_mouse_position().y;
    }
    return true;
  }
  
  bool handle_event(MouseWheelEvent mwheel){
    scale /= powf(1.2,mwheel.change);
    std::cout << scale << "\n";
    return true;
  }
};

class Drawable{
public:
  
  double x, y, w, h;
  double r,g,b;
  virtual void draw(){
    naiveShader->pos(x,y);
    naiveShader->scale(w,h);
    naiveShader->color(r,g,b,1);
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

class NodeExpander{

};

typedef Node<std::list<Drawable *>,2> DrawNode;
typedef Node<int,2> IntNode;

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
    while((a.w>a.h ? a.w : a.h) > 1){
      relativeOrigin = GoUp(a,relativeOrigin);
    }
    int mx = floor(a.x);
    int my = floor(a.y);
    relativeOrigin = relativeOrigin->relative_node(vec2(mx,my),true);
    //std::cout << a.x << " " << mx << "\n";
    a.x -= (double)mx;
    a.y -= (double)my;
    
    while((a.h > a.w ? a.h : a.w) < 0.5){
      a.h *= 2;
      a.w *= 2;
      a.x *= 2;
      a.y *= 2;
      //std::cout << a.x << " " << a.y << "\n";
      relativeOrigin = relativeOrigin->get_child(vec2(a.x,a.y),true);
      //std::cout << "Rel orign: " << relativeOrigin << "\n";
      int mx = relativeOrigin->idx & 1;
      int my = relativeOrigin->idx >>1;
      a.x -= mx;
      a.y -= my;
      
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
    a.w = a.w / 2.0;
    a.h = a.h / 2.0;
    a.x = (a.x +(double) x) / 2.0;
    a.y = (a.y +(double) y) / 2.0;
    return node->get_parent(true);
  }
  void RenderFromOrigin(double x, double y, double w, double h){
    rect b = {x,y,w,h};
    rect screen = {0,0,1,1};
    DrawNode * relativeOrigin = GetNodeForRect(b);
    //relativeOrigin = GoUp(b,relativeOrigin);
    //std::cout << "Camera: ";
    //ptRect(b);
    //std::cout << "\n";
    naiveShader->cameraPosition(b.x,b.y);
    naiveShader->cameraScale(b.w,b.h);
    for(int i = -1;i<2;i++){
      for(int j = -1;j<2;j++){
	RenderDown(relativeOrigin->relative_node(vec2(i,j),true), {0,0,1,1},rect({-b.x+i,-b.y+j,b.w,b.h}));
      }
    }
  }

  void RenderDown(DrawNode * nd, rect cameradef, rect camera){
    if(nd->Data.size() > 0){
      naiveShader->cameraPosition(camera.x,camera.y);
      naiveShader->cameraScale(camera.w,camera.h);
      for(auto it = nd->Data.begin(); it != nd->Data.end(); it++){
	(*it)->draw();
      }
    }
    DrawNode * nd2;
    
    for(int x = 0; x < 2;x++){
      for(int y = 0; y < 2;y++){
	nd2 = nd->get_child(vec2(x,y),false); 
	if(nd2 != NULL){					
	  rect subC = Intersect(cameradef,rect({(double)x,(double)y,0.5,0.5}));	
	  if(subC.w > 0 && subC.h > 0){				
	    subC.x *= 2; 
	    subC.y *= 2; 
	    subC.w *= 2; 
	    subC.h *= 2; 
	    RenderDown(nd2,subC,{(2.0*camera.x +(double)x),(2.0*camera.y +(double) y),camera.w*2.0,camera.h*2.0});
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
 
  //return 0;

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
  for(double i = 0; i < 2000;i += 1){
    for(double j =0; j < 2000;j+=1){
    Drawable* dw = new Drawable();
    dw->x = 1.0*i;
    dw->y = 1.0*j;
    dw->w = 1.0;
    dw->h = 1.0;
    dw->r = sin(i*j/10.0)*0.5 + 0.5;
    dw->g = sin(i*j/10.0 + 1)*0.5 + 0.5;
    dw->b = sin(j*i/10.0 + 2)*0.5 + 0.5;
    world.AddObject(dw);
    }
  }
  //world.Print();
  NaiveShader ns;
  ns.pos(-0.5,-0.5);
  ns.scale(1,1);
  ns.cameraScale(2,2);
  ns.cameraPosition(0,0);
  float t = -1.0;
  vbo.BindBuffer(0);
  Listener1 listener(800,600);
  key_event_handler.register_listener(&listener);
  mouse_click_handler.register_listener(&listener);
  mouse_wheel_event_spawner.register_listener(&listener);
  mouse_move_spawner.register_listener(&listener);
  window_resize_event.register_listener(&listener);
  while(UpdateGraphics()){
    t += 0.01;
    glViewport(0,0,listener.viewport_width, listener.viewport_height);
    world.RenderFromOrigin(listener.x,listener.y,listener.scale,listener.scale);
  }
}
