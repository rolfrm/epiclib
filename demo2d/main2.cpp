
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
#include "../Utils/semi_virtual_ndtree.h"
using namespace std;


using Color = Vec<unsigned char,4>;

Color color(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
  return vec( r ,g, b,  a);
}

Color color(Vec<float,4> f){
  return (f * 255.0f).As<unsigned char>();
}

Color col(Vec<unsigned char,4> colors){
  return color(colors[0],colors[1],colors[2],colors[3]);
} 

/*Color color(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
  return vec( ((float) r) / 255 ,((float) g) / 255, ((float) b) / 255, ((float) a) / 255);
}

Color color(Vec<float,4> f){
  return f;
}

Color col(Vec<unsigned char,4> colors){
  return color(colors[0],colors[1],colors[2],colors[3]);
  } */

unsigned int colorVecToUint(Color color){
  unsigned int colori = 0;
  for(int i = 0; i < 4;i++){
    colori |= ((unsigned int)(color[i]) << (i*8));
  }
  return colori;
}

Color UintToColorVec(unsigned int coli){
  Color * c=(Color *) &coli;
  return *c;
}


class ColorTree: 
  public VNode<Color,2,ColorTree> {
public:
  
  static void PostProcessChild(ColorTree * child){
    child->Data = child->parent->Data;
  }

  static void PostProcessParent(ColorTree * parent, ColorTree * madeFrom){
    //parent->Data = madeFrom->Data;
  }

};


//using QuadTree = Node<Vec<float,4> ,2>;
using QuadTree = ColorTree;

using TextureNode = Node<Texture,2>;

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



void genQuadTreeRec(QuadTree * qt, int lv){
  for(int i = 0; i < 4;i++){
    if(qt->parent != NULL){
      qt->get_child(i,true)->Data = qt->parent->Data;
    }
  }
  switch(rand()%3){
  case 0:
    qt->Data = color(vec(0.5f,0.5f,0.0f,1.0f));break;
  case 1:
    qt->Data = color(vec(0.5f,1.0f,0.0f,1.0f));break;
  case 2:
    qt->Data = color(vec(0.5f,0.2f,1.0f,1.0f));break;
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


#define QuadTreeScale 8
#define QuadTreeSize (1 << QuadTreeScale)
bool RenderQuadToImage(QuadTree * qt, unsigned int * bytes, int width, int height, Vec<int,2> p, int s){
  if(s == 0){
    
    return false;
  }
  
  if(p[0] + s < 0|| p[1] + s < 0 || p[0] >= width || p[1] >= height){
    return false;
    }
  if(s == 1 || !qt->HasChildren()){
    unsigned int color = colorVecToUint(qt->Data);
    if(color == 0){
      
      return false;
    }
  
    double rx = s;
    double ry = s;
  
    if(p[0] < 0){
      rx = rx + p[0];
      p[0] = 0.0;
    }

    if(p[1] < 0){
      ry = ry + p[1];
      p[1] = 0;
    }
    if(p[0] + rx >= width){
      rx = width - p[0];
    }
    if(p[1] + ry >= height){
      ry = height - p[1];
    }
    int x = p[0] ;
    int y = p[1] ;
    int w = rx ;
    int h = ry ;
    for(int yi = y; yi < y + h; yi++){
      for(int xi = x; xi < x + w; xi++){
	bytes[xi + yi*width] = color;
      }
    }
    return true;
  }
  s = s >> 1;
  bool drewSomething = false;
  for(int i = 0; i < 4;i++){
    QuadTree * child = qt->children[i];
    
    if(child == NULL){
      
      unsigned int color = colorVecToUint(qt->Data);
      if(color != 0){
      
	drewSomething = true;
	Vec<int,2> p2 = p + vec<int>(i & 1 , i >> 1)*s;
	
	double rx = s;
	double ry = s;
    
	if(p2[0] < 0){
	  rx = rx + p2[0];
	  p2[0] = 0.0;
	}

	if(p2[1] < 0){
	  ry = ry + p2[1];
	  p2[1] = 0;
	}
	if(p2[0] + rx > width){
	  rx = width - p2[0];
	}
	if(p2[1] + ry > height){
	  ry = height - p2[1];
	}
	int x = p2[0] ;
	int y = p2[1] ;
	int w = rx ;
	int h = ry ;
      
	for(int yi = y; yi < y + h; yi++){
	  for(int xi = x; xi < x + w; xi++){
	    bytes[xi + yi*width] = color;
	  }
	}
      }
    }else{
      drewSomething |= RenderQuadToImage(child, bytes, width, height, p + vec<int>(child->idx & 1 , child->idx >> 1)*s,s);
    }
  }
  return drewSomething;
}

bool RenderQuadtreeToImage(QuadTree * qt, unsigned int * bytes, int outwidth, int outheight, Vec<int,2> offset){
  
  Vec<int,2> p;
  int s = QuadTreeSize;
  while(qt->parent != NULL){
    if(p[0] <= offset[0] && p[1] <= offset[1] && p[0] + s >= offset[0] +QuadTreeSize && p[1] + s >= offset[1] +QuadTreeSize){
      break;
    }
    
    p = p - vec<int>(qt->idx & 1, qt->idx >> 1) * s;
    s = s << 1;
    qt = qt->parent;
  }
  return RenderQuadToImage(qt,bytes,outwidth,outheight,p - offset,s);
}

void recGen2(QuadTree * qt,int lvs){
  if(lvs == 0){
    return;
  }
  qt->get_child(0,true)->Data = color(vec(0.0f,0.3f,0.0f,1.0f));
  qt->get_child(1,true)->Data = color(vec(0.0f,0.5f,0.0f,1.0f));
  qt->get_child(2,true)->Data = color(vec(0.0f,0.6f,0.0f,1.0f));
  recGen2(qt->get_child(3,true),lvs-1);
}

QuadTree * genQuadTree(){
  QuadTree * qt = new QuadTree();
  recGen2(qt,10);
  qt->relative_node(vec(1,1),true)->Data = color(vec(0.0f,0.0f,0.5f,1.0f));
  //return qt;
  return qt->get_child(3,true)->get_child(3,true)->get_child(0,true);
}
#define MAX(x,y) (x > y?x:y)

void Serialize(QuadTree * qt, ostream & ostr){
  unsigned int color = colorVecToUint(qt->Data);
  unsigned char children = 0;
  for(int i = 0; i < 4; i++){
    QuadTree * child = qt->children[i];
    if(child == NULL){
      children |= 1 << i;
      continue;
    }
    if(child->HasChildren()){
      children |= 1 << (i+4);
    }
  }
  ostr.write((char *) &color,sizeof(unsigned int));
  ostr.write((char *) &children,sizeof(unsigned char));
  for(int i = 0; i < 4;i++){
    QuadTree * child = qt->children[i];
    if(child != NULL){
      if(!child->HasChildren()){
	ostr.write((char *) &child->Data,4);
      }else{
	Serialize(child,ostr);
      }
    }
  }
}

QuadTree * Deserialize(istream & istr){
  QuadTree * newQuad = new QuadTree();
  unsigned char children;
  istr.read((char *) &(newQuad->Data),sizeof(Color));
  istr.read((char *) &children,1);
  
  for(int i = 0; i < 4;i++){
    bool isNull = (children & (1 << i)) > 0;
    bool hasChildren = (children & (1 << (i+4))) > 0; 
    if(isNull){
      continue;
    }
    QuadTree * child;
    if(hasChildren){
      child = Deserialize(istr);
    }else{
      child= new QuadTree();
      istr.read((char *) &(child->Data),sizeof(Color));
    }
    child->idx = i;
    newQuad->children[i] = child;
    child->parent = newQuad;
  }
  
  return newQuad;
}

unsigned int median4(unsigned int a, unsigned int b, unsigned int c, unsigned int d){
  unsigned int low1 = b, low2 = a,high1 = d,high2 = c;
  if( a < b){
    low1 = a;
    high1 = b;
  }
  if( c < d){
    low2 = c;
    high2 = d;
  }

  if(low1 > low2){
    low2 = low1;
  }
  if(high1 > high2){
    high2 = high1;
  }
  return low2;

}


class QuadtreeRenderer{
public:
  QuadTree * origin;
  Vec<double,2> LocalP; // Screen upper left corner
  double Zoom;
  Texture2DShader texShader;
  VBO SquareBuffer;
  int chunkSize;
  int chunkScale;
  Vec<int,2> ScreenSize;
  TextureNode* renderTree;
  unsigned int * byteBuffer;
  Texture nullTex;
  bool Change;
  int renderit;
  QuadtreeRenderer(QuadTree * _origin,VBO squareBuffer, Texture2DShader tex2DShader, Vec<int,2> screenSize):
    SquareBuffer(squareBuffer), 
    texShader(tex2DShader)
    
  {
    Change = true;
    ScreenSize = screenSize;
    origin = _origin;
    Zoom = 1.0;
    LocalP = vec(0.0,0.0);
    chunkScale = QuadTreeScale;
    chunkSize = 1 << chunkScale;
    renderTree = new TextureNode();
    byteBuffer = new unsigned int[chunkSize*chunkSize];
    unsigned int def[]{0x00000000,0x0,0x0,0x0};
    nullTex = Texture(2,2,def);
    renderit = 0;
  }
  map<TextureNode *, int> texturesInUse;
  void Render(){
    renderit += 1;
    Change = false;
    Vec<double,2> chunkScreenSize = vec<double>(chunkSize,chunkSize) / ScreenSize.As<double>()*Zoom;
    int nRenders = 0;
    Vec<double,2> chunkScale = LocalP / chunkSize;
    Vec<int,2> start = Floor(chunkScale).As<int>();
    Vec<int,2> end = (start.As<double>() + ScreenSize.As<double>() / (double)chunkSize / Zoom ).As<int>() + 1;
    Vec<double,2> offset = (chunkScale - start.As<double>());
    texShader.SetSize(chunkScreenSize[0],chunkScreenSize[1]);
    SquareBuffer.Bind(0);
    texShader.SetUVState(vec(0.0,0.0),vec(1.0,1.0));
    for(int x = start[0]; x <= end[0];x++){
      for(int y = start[1];y <= end[1];y++){
	Vec<int,2> tpos = vec<int>(x,y)*chunkSize;
	TextureNode * imgNode = renderTree->relative_node(vec(x,y),true);
	Texture tex = imgNode->Data;
	if(!tex.HasData() && nRenders++ < 20){
	  bool ok = RenderQuadtreeToImage(origin,byteBuffer,chunkSize,chunkSize, tpos.As<int>());
	  if(ok){
	    tex = Texture(chunkSize,chunkSize,byteBuffer,Interpolation::Linear,TextureWrap::ClampToEdge);
	    fill(byteBuffer,byteBuffer + chunkSize*chunkSize,0);
	    imgNode->Data = tex;
	  }else{
	    nRenders--;
	  }
	}
	if(nRenders >= 20){
	  Change = true;
	}
	Vec<double,2> objPos =vec((x - chunkScale[0])*chunkScreenSize[0] , (y - chunkScale[1])*chunkScreenSize[1]);
	bool uvstateChanged = false;
	if(!tex.HasData()){
	  //try to fetch texture from different layer options: go down and render 2x2 or go up and render a chunk(prefered)
	  if(imgNode->parent->Data.HasData()){
	    int idx = imgNode->idx;
	    tex = imgNode->parent->Data;
	    texturesInUse[imgNode->parent] = renderit;
	    texShader.SetUVState(vec<double>(idx & 1, idx >> 1) / 2.0,vec(0.5,0.5));
	    uvstateChanged = true;
	  }else{
	    nullTex.Bind(0);
	  }
	}else{
	  texturesInUse[imgNode] = renderit;
	  tex.Bind(0);
	}
	texShader.SetPos(objPos[0], objPos[1]);
   	VBO::DrawBuffers(DrawMethod::Quads,4);
	texShader.SetUVState(vec(0.0,0.0),vec(1.0,1.0));
      }
    }

    list<TextureNode *> toRemove;
    for(auto it = texturesInUse.begin();it != texturesInUse.end();it++){
      if(it->second < renderit){
	toRemove.push_back(it->first);
      }
    }
    for(TextureNode * node : toRemove){
      node->Data.Release();
      texturesInUse.erase(node);
    }

  }

  void _repaint_down(TextureNode * cell){
    cell->Data.Release();
    for(int i = 0; i < 4;i++){
      if(cell->children[i] != NULL){
	_repaint_down(cell->children[i]);
      }
    }
  }

  void Repaint(TextureNode * cell){
    TextureNode * cell2 = cell->parent;
    while(cell2 != NULL){
      cell2->Data.Release();
      cell2 = cell2->parent;
    }
    _repaint_down(cell);
  }

  std::map<QuadTree * ,bool> paintedNodes;

  void finishedPainting(){
    for(auto it = paintedNodes.begin();it != paintedNodes.end();it++){
      optimizeTree(it->first);
    }
    paintedNodes = map<QuadTree* , bool>();
  }

  Color sampleColor(Vec<int,2> point){
    Vec<double,2> worldPos = ScreenToWorld(point); //* Zoom;
    QuadTree * end = TraceDown(worldPos);
    //end->Data = color;
    int cellsize = QuadTreeSize;
    Vec<double,2> rendercell = Floor(worldPos / chunkSize);
    TextureNode * rcell = renderTree->relative_node(rendercell.As<int>(),false);
    Repaint(rcell);
    
    Vec<double,2> cell = Floor(worldPos / QuadTreeSize);
    Vec<double,2> rest = (worldPos/ QuadTreeSize - cell)*2.0;
    QuadTree * node =  origin->relative_node(cell.As<int>(),true);
    QuadTree * firstNode = node;
    while(cellsize > 1){
      Vec<int,2> newcell = rest.As<int>();
      rest = (rest - newcell.As<double>())*2.0;
      cellsize >>= 1;
      QuadTree * newnode = node->get_child(newcell,true);
      node = newnode;
    }
    return node->Data;
  }

  void PaintDotScreen(Vec<int,2> point,Color color, int pxsize){
    Change = true;
    Vec<double,2> worldPos = ScreenToWorld(point); //* Zoom;
    QuadTree * end = TraceDown(worldPos);
    //end->Data = color;
    int cellsize = QuadTreeSize;
    Vec<double,2> rendercell = Floor(worldPos / chunkSize);
    TextureNode * rcell = renderTree->relative_node(rendercell.As<int>(),false);
    Repaint(rcell);
    
    Vec<double,2> cell = Floor(worldPos / QuadTreeSize);
    Vec<double,2> rest = (worldPos/ QuadTreeSize - cell)*2.0;
    QuadTree * node =  origin->relative_node(cell.As<int>(),true);
    QuadTree * firstNode = node;
    int downit = 0;
    while(cellsize > 1){
      downit +=1;
      Vec<int,2> newcell = rest.As<int>();
      rest = (rest - newcell.As<double>())*2.0;
      cellsize >>= 1;
      QuadTree * newnode = node->get_child(newcell,true);
      node = newnode;
    }
    pxsize /=Zoom;
    paintedNodes[firstNode] = true;
    for(int i = -pxsize/2;i < pxsize/2+1;i++){
      for(int j = -pxsize/2;j < pxsize/2+1;j++){
	QuadTree * qn = node->relative_node(vec(i,j),true);
	qn->DeleteChildren();
	qn->Data = color;
	
      }
    }
  }
  void ZoomIn(double Change, Vec<double,2> Around){
    Vec<double,2> d = (Around - LocalP)/Change;
    Vec<double,2> finalChange = d * (Change - 1);
    Zoom *= Change;
    LocalP =LocalP + finalChange;
    UpdateState();
  }

  void optimizeTree(QuadTree * cell){
    unsigned int returnedColor[4];
    bool hasChildren = false;
    bool hasNull = false;
    for(int i = 0; i < 4;i++){
      QuadTree * child = cell->children[i];
      if(child == NULL){
	hasNull = true;
	returnedColor[i] = colorVecToUint(cell->Data);
	continue;
      }
      if(child->HasChildren()){
	optimizeTree(child);
      }
      returnedColor[i] = colorVecToUint(child->Data);
      if(!child->HasChildren()){
	if(cell->Data == child->Data){
	  delete child;
	  cell->children[i] = NULL;
	}
      }

      if(child == NULL){
	hasNull = true;
      }
    }
    //Color finalColor = UintToColorVec(median4(returnedColor[0],returnedColor[1],returnedColor[2],returnedColor[3]));
    
  }
    
  Vec<double,2> ScreenToWorld(Vec<int,2> screenPos){
    return vec<double>(-screenPos[0], ScreenSize[1] - screenPos[1]) / Zoom + LocalP;
  }

  void ZoomInAroundScreenPos(double Change, Vec<int,2> pos){
    ZoomIn(Change, ScreenToWorld(pos));
  }

  void Move(Vec<double,2> amount){
    LocalP =LocalP + amount / Zoom ;
    UpdateState();
  }

  QuadTree * TraceDown(Vec<double,2> position){
    QuadTree * qt = origin;
    int s = QuadTreeSize;

    while(position[0] > s || position[1] > s || position[0] < 0 || position[1] < 0){
      position = position / 2.0 + vec(qt->idx & 1, qt->idx >> 1).As<double>() * s;
      qt = qt->get_parent(true);
      s *= 2;
    }
    print(position);
    while(s > 1){
      Vec<double,2> f = Floor(position / (double)chunkSize);
      position = (position - f)*2.0;
      QuadTree * n = qt->relative_node(f.As<int>(),false);
      if(n == NULL){
	break;
      }
      qt = n;
    }
    return qt;
  }

  void SetWindowSize(Vec<int,2> size){
    ScreenSize = size;
    Change = true;
  }

  void UpdateState(){
    Change = true;
    bool updateAgain = false;
    Vec<int,2> p = LocalP.As<int>() >> QuadTreeScale;
    if(p[0] != 0 || p[1] != 0){
      Vec<int,2> relp = p << (QuadTreeScale - chunkScale);
      QuadTree * newOrigin = origin->relative_node(p,true);
      if(newOrigin != NULL ){
	updateAgain = true;
	renderTree = renderTree->relative_node(relp,true);
	origin = newOrigin;
	Vec<double,2> move = (p << QuadTreeScale).As<double>();
	LocalP = LocalP - move;
      }
    }
    
    int idx = origin->idx;
    int rIdx = renderTree->idx;
    Vec<int,2> rcell = vec(rIdx &1, rIdx>>1);
    Vec<int,2> cell = vec(idx & 1, idx >> 1);
    if(Zoom < 0.5){
      updateAgain = true;
      origin = origin->get_parent(true);
	
      renderTree = renderTree->get_parent(true)->relative_node(cell-rcell,true);
      LocalP = LocalP / 2.0 + (cell * QuadTreeSize).As<double>()/2.0;
      Zoom *= 2.0;
    }else if(Zoom > 1.0){
      QuadTree * newOrigin = origin->get_child(cell,true);
      if(newOrigin != NULL){
	updateAgain = true;
	origin = newOrigin;
	renderTree = renderTree->get_child(cell,true);
	
	LocalP = LocalP * 2.0 - (cell * QuadTreeSize).As<double>();
	Zoom /= 2.0;
      }
    }
    if(updateAgain){
      UpdateState();
    }
  }
  
    
};
Vec<float,4> getCurrentColor();
void setCurrentColor(Vec<unsigned char,4> color);
int getCurrentBrushSize();
int getCurrentSize();
class SimpleEvents:
  public EventListener<KeyEvent>,
  public EventListener<MouseClick>,
  public EventListener<MouseWheelEvent>,
  public EventListener<mouse_position>,
  public EventListener<size>
{
public:
  
  bool leftDown;
  bool rightDown;
  bool ctrlDown;
  bool first;
  Vec<int,2> last;
  vec<int,2> BrushPosition;
  QuadtreeRenderer * renderer;

  bool Running;
  Texture2DShader texShader;
  VBO SquareBuffer;
  Texture nullTex;

  SimpleEvents(QuadtreeRenderer * qtRenderer,Texture2DShader shader, VBO squareBuffer):
    texShader(shader),
    SquareBuffer(squareBuffer)
  {
    unsigned int nulltex[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
    nullTex = Texture(2,2,nulltex,Interpolation::Nearest);
    renderer = qtRenderer;
    leftDown = false;
    rightDown = false;
    Running = true;
    ctrlDown = false;
  }

  bool handle_event(KeyEvent kev){
    std::cout << kev.key << "\n";
    if(kev.key == ESC || kev.key == 'Q'){
      Running = false;
    }
    if(kev.key == CTRL){
      ctrlDown = kev.pressed;
    }
  }
  bool handle_event(MouseClick mclick){
    if(mclick.button == 0){
      leftDown = mclick.pressed;
      first = true;
      if(mclick.pressed){
	if(!KeyIsDown(CTRL)){

	renderer->PaintDotScreen(last,color(getCurrentColor()),getCurrentSize());
	}else{
	  Color col = renderer->sampleColor(last);
	  setCurrentColor(col);
	}
      }else{
	renderer->finishedPainting();
      }
    }
    if(mclick.button == 1){
      rightDown = mclick.pressed;
      
    }
  }
  bool handle_event(MouseWheelEvent mwheel){
    double zoomAmount = 1.05;
    renderer->ZoomInAroundScreenPos(mwheel.change > 0? zoomAmount : 1.0 / zoomAmount,last);
  }
  bool handle_event(mouse_position mpos){
    Vec<int,2> pos = vec(-mpos.x,mpos.y);
    Vec<int,2> dp = pos - last;
    if(leftDown){  
      if(!KeyIsDown(CTRL)){

      renderer->PaintDotScreen(last,color(getCurrentColor()),getCurrentSize());
    }else{
      Color col = renderer->sampleColor(last);
      setCurrentColor(col);
    }
    }

    if(rightDown){
      renderer->Move(dp.As<double>());
    }
    
    last = pos;
    return true;
  }
  bool handle_event(size s){
    renderer->SetWindowSize(vec(s.x,s.y));
  }

  
  void RenderBrushPreview(){
    texShader.SetUVState(vec(0.0,0.0),vec(1.0,1.0));
    Vec<double,2> size = vec<double>(getCurrentSize(),getCurrentSize()) / renderer->ScreenSize.As<double>();
    Vec<double,2> pos = BrushPosition.As<double>() / renderer->ScreenSize.As<double>() + size / 2.0;
    
    texShader.SetPos(-pos[0],1.0 - pos[1]);;
    
    texShader.SetSize( size[0],size[1]);
    SquareBuffer.Bind(0);
    nullTex.Bind(0);
    Vec<float,4> col = getCurrentColor();
    col[3] = 0.9;
    texShader.SetColor(color(col));
    VBO::DrawBuffers(DrawMethod::Quads,4);
    texShader.SetColor(color(255,255,255,255));
  }
};

void writeQuadTreeState(QuadTree * qt, ostream & str){
  std::list<int> stepsUp;
  while(qt->parent != NULL){
    stepsUp.push_back(qt->idx);
    qt = qt->parent;
  }
  stepsUp.reverse();
  int size = stepsUp.size();
  str.write((char *) &size,sizeof(int));
  
  for(auto it = stepsUp.begin();it != stepsUp.end();it++){
    int step = *it;
    str.write((char *) &step,sizeof(int));
  
  }
  Serialize(qt,str);
}

QuadTree * loadQuadtreeState(istream & str){
  int size;
  str.read(( char *) &size, sizeof(int));
  std::list<int> stepsDown;
  for(int i = 0; i < size;i++){
    int in;
    str.read((char *) &in, sizeof(int));
    stepsDown.push_back(in);
  }
  QuadTree * tree = Deserialize(str);
  for(auto it = stepsDown.begin(); it != stepsDown.end();it++){
    std::cout << "Going down.. \n";
    tree = tree->children[*it];
  }
  return tree;
}

#define SCREENWIDTH 700
#define SCREENHEIGHT 512
#include<fstream>
int test_main(){
  Texture2DShader texShader;
  fstream fstr;
  fstr.open("test.save", ios::in);
  QuadTree * qt;
  if(!fstr.good()){
    qt = genQuadTree();
  }else{
    qt = loadQuadtreeState(fstr);
  }
  fstr.close();
  
  Context context;
  float squaredata[] = {0,0,
			1.0,0.0,
			1.0,1.0,
			0.0,1.0};
  VBO SquareBuffer(squaredata,4,2,VBODrawType::Static);
  QuadtreeRenderer qtr(qt,SquareBuffer,texShader,vec(SCREENWIDTH,SCREENHEIGHT));
  SimpleEvents sev(&qtr,texShader,SquareBuffer);
  key_event_handler.register_listener(&sev);
  mouse_click_handler.register_listener(&sev);
  mouse_move_spawner.register_listener(&sev);
  mouse_wheel_event_spawner.register_listener(&sev);
  window_resize_event.register_listener(&sev);
  qtr.Move(vec(400.0,-25.0));
  int x = 0;
  while(sev.Running){
    StopWatch swatch;
    swatch.Reset();
    swatch.Start();
    //if(qtr.Change){
      FrameBuffer::screenBuffer.Clear();
      qtr.Render();
      //}
    sev.RenderBrushPreview();
    Sleep(0.01);

    SwapBuffers();
    
    
  }
  fstr.open("test.save", ios::out | ios::trunc);
  writeQuadTreeState(qtr.origin,fstr);
  fstr.close();
  std::cout << "Ending\n";
  return 0;
}
#include <thread>
void uimain();
int main(){
  std::thread t(uimain);
  initOpenGL(SCREENWIDTH,SCREENHEIGHT);
  init_events();

  return test_main();
}
