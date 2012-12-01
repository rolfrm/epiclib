#include "../Utils/ndtree.h"
#include "../Math/GenericVector.h"
#include "../Graphics/GraphicsCore.hpp"
#include "../Utils/Timing.h"
#include "../Graphics/Texture.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Debug.h"
#include "../Graphics/Framebuffer.hpp"
#include "../Events/input_events.h"
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
using namespace std;

using QuadTree = Node<Vec<float,4> ,2>;

Texture CreateTextureFromQuadTree(QuadTree * qt, int lod){
  int size = 1 << lod;
  int * data = new int[size*size];
  
  Texture out(size,size,data);
  delete data;
  return out;
}

class Texture2DShader{

  Program shader;
public:
  Texture2DShader():shader(Shader::FromFile("tex2DShader.vert",ShaderType::Vertex),
			   Shader::FromFile("tex2DShader.frag",ShaderType::Fragment)){

  }

  void SetPos(double x, double y){
    shader.UseProgram();
    shader.setUniform("position",x,y);
  }

  void SetSize(double w, double h){
    shader.UseProgram();
    shader.setUniform("size",w,h);
  }

  void SetTexture(unsigned int channel){
    shader.UseProgram();
    shader.setUniform("tex",(int)channel);
  }

};

class Flat2DColorShader{
  Program shader;
public:
  Flat2DColorShader():shader(Shader::FromFile("flat2dcolorshader.vert",
					      ShaderType::Vertex),
			     Shader::FromFile("flat2dcolorshader.frag",
					      ShaderType::Fragment)){

  }
  void SetState(double x, double y, double s){
    shader.UseProgram();
    shader.setUniform("x",x);
    shader.setUniform("y",y);
    shader.setUniform("s",s);
    shader.setUniform("width",(int)300);
    shader.setUniform("height",(int)300);
  }
  void SetColor(Vec<float,4> color){
    shader.UseProgram();
    shader.setUniform("color", color);
  }
};

class Context{
public:
  int ScreenWidth;
  int ScreenHeight;
  Flat2DColorShader flat2DColorShader;
  VBO SquareBuffer;
  
};

void renderQuadrant(Context & context, QuadTree * tree, double x, double y, double s){
  if(1 / s > context.ScreenWidth){
    return;
  }
  if(1 / s == context.ScreenWidth || !tree->HasChildren()){
    Flat2DColorShader sh = context.flat2DColorShader;
    sh.SetState(x,y,s);
    sh.SetColor(tree->Data);
    context.SquareBuffer.Bind(0);
    VBO::DrawBuffers(DrawMethod::Quads,4);
    return;
  }
  for(QuadTree * child : tree->children){
    if(child == NULL) {
      continue;
    }
    int idx = child->idx;
    double nx = (idx & 1)* s + x;
    double ny = (idx >> 1)* s + y;
    renderQuadrant(context,child,nx,ny,s/2);
  }
}

//Assume render fullscreen at max lod
//origin is lod=0 -> size 1,1
void NaiveRenderQuadTree(Context & context, QuadTree * origin,double offx,double offy)
{
  //Go up till fill screen or end
  double s = 1.0;
  double ox = 0.0;
  double oy = 0.0;
  
  while(origin->parent != NULL && ox < offx && oy < offy && s < offx + 1.0 - ox && s < offy + 1.0 - oy){
    int idx = origin->idx;
    ox += (idx & 1) * s;
    oy += (idx >> 1) * s;
    origin = origin->parent;
  }
  
  renderQuadrant(context,origin,ox,oy,s);
}

void genQuadTreeRec(QuadTree * qt, int lv){
  for(int i = 0; i < 4;i++){
    if(qt->parent != NULL){
      qt->get_child(i,true)->Data = qt->parent->Data;
    }
  }
  switch(rand()%3){
  case 0:
    qt->Data = vec(0.5f,0.5f,0.0f,1.0f);break;
  case 1:
    qt->Data = vec(0.5f,1.0f,0.0f,1.0f);break;
  case 2:
    qt->Data = vec(0.5f,0.2f,1.0f,1.0f);break;
  }
 if(lv == 0){
    return;
  }
  for(int i = 1;i < 4;i++){
    genQuadTreeRec(qt->get_child(i,true),lv - 1);
  }
}

void RenderQuadTree(Vec<double,2> topLeft,Vec<double,2> pxSize,QuadTree * origin){

}


class QuadTreeRenderer{
public:
  
#define CHUNKSIZE 512
  class StoredRendering{
  public:
    Vec<double,2> position;
  };

  double zoom;
  Vec<double, 2> p;
  
  QuadTreeRenderer(QuadTree * qt){
    zoom = 1.0;
    p = vec(0.0,0.0);
  }

  void Render(){
    
  }

  void Zoom(double amount){
    zoom *= amount;
  }
  
  void Move(Vec<double,2> pixels){
    p = p + pixels * zoom;
  }

};

unsigned int colorVecToUint(Vec<float,4> color){
  unsigned int colori = 0;
  for(int i = 0; i < 4;i++){
    colori |= ((unsigned int)(color[i]*255.0f) << (i*8));
  }
  return colori;
}

void RenderQuadToImage(QuadTree * qt, unsigned int * bytes, int width, int height, Vec<double,2> p,double s){
  if(1 / s > width || 1 / s > height){
    return;
  }
  if(p[0] + s < 0.0 || p[1] + s < 0.0 || p[0] > 1.0 || p[1] > 1.0){
    return;
  }
  
  if(1 / s == width || !qt->HasChildren()){
    
    double rx = s;
    double ry = s;
  
    if(p[0] < 0.0){
      rx = rx + p[0];
      p[0] = 0.0;
    }
    if(p[1] < 0.0){
      ry = ry + p[1];
      p[1] = 0;
    }
    if(p[0] + rx > 1.0){
      rx = 1.0 - p[0];
    }
    if(p[1] + ry > 1.0){
      ry = 1.0 - p[1];
    }
    int x = p[0] * width;
    int y = p[1] * height;
    int w = rx * width;
    int h = ry * height;
    unsigned int color = colorVecToUint(qt->Data);
    for(int yi = y; yi < y + h; yi++){
      for(int xi = x; xi < x + w; xi++){
	bytes[xi + yi*width] = color;
      }
    }
  }
  s /= 2;
  for(QuadTree * child: qt->children){
    if(child == NULL){
      continue;
    }
    
    RenderQuadToImage(child, bytes, width, height, p + vec<double>(child->idx & 1 , child->idx >> 1)*s,s);
  }
}

void RenderQuadtreeToImage(QuadTree * qt, unsigned int * bytes, int width, int height, Vec<double,2> offset){
  double s = 1.0;
  Vec<double,2> p = vec(0.0,0.0);
  while(qt->parent != NULL){
    if(p[0] < offset[0] && p[1] < offset[1] && p[0] + s > offset[0] + 1 && p[1] + s > offset[1] + 1){
      break;
    }
    p[0] -= (qt->idx & 1) * s;
    p[1] -= (qt->idx >> 1) * s;
    s *= 2;
    qt = qt->parent;
  }
  RenderQuadToImage(qt,bytes,width,height,p - offset,s);
}

void recGen2(QuadTree * qt,int lvs){
  if(lvs == 0){
    return;
  }
  qt->get_child(0,true)->Data = vec(0.0f,0.3f,0.0f,1.0f);
  qt->get_child(1,true)->Data = vec(0.0f,0.5f,0.0f,1.0f);
  qt->get_child(2,true)->Data = vec(0.0f,0.6f,0.0f,1.0f);
  recGen2(qt->get_child(3,true),lvs-1);
}

QuadTree * genQuadTree(){
  QuadTree * qt = new QuadTree();
  recGen2(qt,10);
  //return qt;
  return qt->get_child(3,true)->get_child(3,true)->get_child(0,true);
}
#define MAX(x,y) (x > y?x:y)

class QuadtreeRenderer{
public:
  QuadTree * origin;
  Vec<double,2> LocalP; // Screen upper left corner
  double Zoom;
  Texture2DShader texShader;
  VBO SquareBuffer;
  int chunkSize;
  Vec<int,2> ScreenSize;
  Node<Texture,2>* renderTree;
  unsigned int * byteBuffer;
  Texture nullTex;
  QuadtreeRenderer(QuadTree * _origin,VBO squareBuffer, Texture2DShader tex2DShader, Vec<int,2> screenSize):
    SquareBuffer(squareBuffer), 
    texShader(tex2DShader)
    
  {
    ScreenSize = screenSize;
    origin = _origin;
    Zoom = 1.0;
    LocalP = vec(0.0,0.0);
    chunkSize = 32;
    renderTree = new Node<Texture,2>();
    byteBuffer = new unsigned int[chunkSize*chunkSize];
    unsigned int def[]{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
    nullTex = Texture(2,2,def);
  }

  void Render(){
    Vec<double,2> chunkScreenSize = vec<double>(chunkSize,chunkSize) / ScreenSize.As<double>()*Zoom;
    int nRenders = 0;
    Vec<double,2> chunkScale = LocalP / chunkSize;
    Vec<int,2> start = Floor(chunkScale).As<int>();
    Vec<int,2> end = (start + ScreenSize / (double)chunkSize) + 1;
    Vec<double,2> offset = (chunkScale - start.As<double>());
    print(chunkScreenSize);
    print(start);
    print(chunkScale);
    texShader.SetSize(chunkScreenSize[0],chunkScreenSize[1]);
    SquareBuffer.Bind(0);
    for(int x = start[0]; x <= end[0];x++){
      for(int y = start[1];y <= end[1];y++){

	Node<Texture,2> * imgNode = renderTree->relative_node(vec(x,y),true);
	Texture tex = imgNode->Data;
	if(!tex.HasData() && nRenders++ == 0){
	  fill(byteBuffer,byteBuffer + chunkSize*chunkSize,0);
	  RenderQuadtreeToImage(origin,byteBuffer,chunkSize,chunkSize, vec(x,y-1).As<double>() * chunkScreenSize);
	  tex = Texture(chunkSize,chunkSize,byteBuffer,Interpolation::Nearest);
	  imgNode->Data = tex;
	}
	if(!tex.HasData()){
	  nullTex.Bind(0);
	}else{
	  tex.Bind(0);
	}
	texShader.SetPos((x - chunkScale[0])*chunkScreenSize[0] , (y - chunkScale[1])*chunkScreenSize[1]);
	VBO::DrawBuffers(DrawMethod::Quads,4);
      }
    }
  }

  void ZoomIn(double Change, Vec<double,2> Around){
    Zoom *= Change;
  }

  void ZoomInAroundScreenPos(double Change, Vec<int,2> pos){
    Zoom *= Change;
  }

  void Move(Vec<double,2> amount){
    LocalP =LocalP + amount / Zoom ;
  }

    
};

class SimpleEvents:
  public EventListener<KeyEvent>,
  public EventListener<MouseClick>,
  public EventListener<MouseWheelEvent>,
  public EventListener<mouse_position>
{
public:
  
  bool leftDown;
  bool first;
  Vec<int,2> last;
  QuadtreeRenderer * renderer;
  SimpleEvents(QuadtreeRenderer * qtRenderer){
    renderer = qtRenderer;
    leftDown = false;
    
  }
  bool handle_event(KeyEvent kev){

  }
  bool handle_event(MouseClick mclick){
    if(mclick.button == 0){
      leftDown = mclick.pressed;
      first = true;
    }
  }
  bool handle_event(MouseWheelEvent mwheel){
    renderer->ZoomInAroundScreenPos(mwheel.change > 0? 1.02 : 0.98,last);
  }
  bool handle_event(mouse_position mpos){
    Vec<int,2> pos = vec(-mpos.x,mpos.y);
    if(leftDown){
      if(first){
	first = false;
	last = pos;
	return false;
      }
      Vec<int,2> dp = pos - last;
      
      renderer->Move(dp.As<double>());
    }
    last = pos;
    return true;
  }
  
};

#define SCREENWIDTH 512
#define SCREENHEIGHT 512
int test_main(){
  Texture2DShader texShader;
  
  QuadTree * qt = genQuadTree();
  Context context;
  float squaredata[] = {0,0,
			1.0,0.0,
			1.0,1.0,
			0.0,1.0};
  QuadtreeRenderer qtr(qt,VBO(squaredata,4,2,VBODrawType::Static),texShader,vec(SCREENWIDTH,SCREENHEIGHT));
  SimpleEvents sev(&qtr);
  key_event_handler.register_listener(&sev);
  mouse_click_handler.register_listener(&sev);
  mouse_move_spawner.register_listener(&sev);
  mouse_wheel_event_spawner.register_listener(&sev);

  while(true){
    StopWatch swatch;
    swatch.Reset();
    swatch.Start();
    FrameBuffer::screenBuffer.Clear();
    qtr.Render();
  
    SwapBuffers();
    
    std::cout << 1.0 / swatch.ElapsedSeconds() << "\n";
    Sleep(0.01);
    
  }
  return 0;
}
int main(){

  initOpenGL(SCREENWIDTH,SCREENHEIGHT);
  init_events();

  return test_main();
}
