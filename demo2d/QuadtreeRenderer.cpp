#include "QuadtreeRenderer.h"
#include <list>
using namespace std;
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
#define TextureNode Node<Texture,2>
QuadtreeRenderer::QuadtreeRenderer(QuadTree * _origin,VBO squareBuffer, Texture2DShader tex2DShader, Vec<int,2> screenSize):
    SquareBuffer(squareBuffer), 
    texShader(tex2DShader)
    
  {
    Change = true;
    ScreenSize = screenSize;
    origin = _origin;
    Zoom = 1.0;
    NewZoom = Zoom;
    LocalP = vec(0.0,0.0);
    NewP = vec(0.0,0.0);
    chunkScale = QuadTreeScale;
    chunkSize = 1 << chunkScale;
    renderTree = new TextureNode();
    byteBuffer = new unsigned int[chunkSize*chunkSize];
    unsigned int def[]{0x00000000,0x0,0x0,0x0};
    nullTex = Texture(2,2,def);
    renderit = 0;
  }
  map<TextureNode *, int> texturesInUse;
  map<TextureNode *, bool> toRelease;

void QuadtreeRenderer::Render(){
    UpdateState();
	 for(auto it = toRelease.begin();it != toRelease.end();it++){
		 it->first->Data.Release();
	 }
	 toRelease = map<TextureNode *,bool>();
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
    texShader.SetColor(color(255,255,255,255));
    for(int x = start[0]; x <= end[0];x++){
      for(int y = start[1];y <= end[1];y++){
	Vec<int,2> tpos = vec<int>(x,y)*chunkSize;
	TextureNode * imgNode = renderTree->relative_node(vec(x,y),true);
	Texture tex = nullTex;
	if(!imgNode->Data.HasData()){// && nRenders++ < 40){
	  bool ok = RenderQuadtreeToImage(origin,byteBuffer,chunkSize,chunkSize, tpos.As<int>());
	  if(ok){
	    tex = Texture(chunkSize,chunkSize,byteBuffer,Interpolation::Linear,TextureWrap::ClampToEdge);
	    fill(byteBuffer,byteBuffer + chunkSize*chunkSize,0);
	    imgNode->Data = tex;
	  }else{
	    nRenders--;
	  }
	}else{
		tex = imgNode->Data;
	}
	/*if(nRenders >= 40){
	  Change = true;
	}*/
	Vec<double,2> objPos =vec((x - chunkScale[0])*chunkScreenSize[0] , (y - chunkScale[1])*chunkScreenSize[1]);
	bool uvstateChanged = false;
	if(!tex.HasData()){
	  //try to fetch texture from different layer options: go down and render 2x2 or go up and render a chunk(prefered)
	  if(imgNode->parent->Data.HasData()){
	    /*int idx = imgNode->idx;
	    tex = imgNode->parent->Data;
	    texturesInUse[imgNode->parent] = renderit;
	    texShader.SetUVState(vec<double>(idx & 1, idx >> 1) / 2.0,vec(0.5,0.5));
	    uvstateChanged = true;*/

		  nullTex.Bind(0);
	  }else{
	    nullTex.Bind(0);
	  }
	  continue;
	}else{
	  texturesInUse[imgNode] = renderit;
	  tex.Bind(0);
	}
	texShader.SetPos(objPos[0], objPos[1]);
   	VBO::DrawBuffers(DrawMethod::TriangleStrip,5);
	texShader.SetUVState(vec(0.0,0.0),vec(1.0,1.0));
      }
    }

    list<TextureNode *> toRemove;
    for(auto it = texturesInUse.begin();it != texturesInUse.end();it++){
      if(it->second < renderit){
	toRemove.push_back(it->first);
      }
    }
    for(auto it = toRemove.begin();it != toRemove.end();it++){
    	(*it)->Data.Release();
    	texturesInUse.erase(*it);
    }
  }

  void QuadtreeRenderer::_repaint_down(TextureNode * cell){
    //cell->Data.Release();
    toRelease[cell] = true;
    for(int i = 0; i < 4;i++){
      if(cell->children[i] != NULL){
	   _repaint_down(cell->children[i]);
      }
    }
  }

void QuadtreeRenderer::Repaint(TextureNode * cell){
    TextureNode * cell2 = cell->parent;
    while(cell2 != NULL){
    	toRelease[cell2] = true;
      cell2 = cell2->parent;
    }
    _repaint_down(cell);
  }

void QuadtreeRenderer::finishedPainting(){
    for(auto it = paintedNodes.begin();it != paintedNodes.end();it++){
      optimizeTree(it->first);
    }
    paintedNodes = map<QuadTree* , bool>();
  }

  Color QuadtreeRenderer::sampleColor(Vec<int,2> point){
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
#include "epiclib3/Utils/Debug.h"
  void QuadtreeRenderer::PaintDotScreen(Vec<int,2> point,Color color, int pxsize){
    Change = true;
    Vec<double,2> worldPos = ScreenToWorld(point);
    int cellsize = QuadTreeSize;
    Vec<double,2> rendercell = Floor(worldPos / chunkSize);
    TextureNode * rcell = renderTree->relative_node(rendercell.As<int>(),true);
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
  void QuadtreeRenderer::ZoomIn(double Change, Vec<double,2> Around){
    Vec<double,2> d = (Around - NewP)/Change;
    Vec<double,2> finalChange = d * (Change - 1);
    NewZoom *= Change;
    NewP =NewP + finalChange;
  }

  void QuadtreeRenderer::optimizeTree(QuadTree * cell){
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
    if(hasNull == false){
    	cell->Data = UintToColorVec(median4(returnedColor[0],returnedColor[1],returnedColor[2],returnedColor[3]));
    }
  }
    
  Vec<double,2> QuadtreeRenderer::ScreenToWorld(Vec<int,2> screenPos){
    return vec<double>(-screenPos[0], ScreenSize[1] - screenPos[1]) / NewZoom + NewP;
  }

  void QuadtreeRenderer::ZoomInAroundScreenPos(double Change, Vec<int,2> pos){
    ZoomIn(Change, ScreenToWorld(pos));
  }

  void QuadtreeRenderer::Move(Vec<double,2> amount){
    NewP =NewP + amount / NewZoom ;
    //UpdateState();
  }

  QuadTree * QuadtreeRenderer::TraceDown(Vec<double,2> position){
    QuadTree * qt = origin;
    int s = QuadTreeSize;

    while(position[0] > s || position[1] > s || position[0] < 0 || position[1] < 0){
      position = position / 2.0 + vec(qt->idx & 1, qt->idx >> 1).As<double>() * s;
      qt = qt->get_parent(true);
      s *= 2;
    }
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

void QuadtreeRenderer::SetWindowSize(Vec<int,2> size){
    ScreenSize = size;
    Change = true;
  }

  void QuadtreeRenderer::UpdateState(){
	LocalP = NewP;
	Zoom = NewZoom;
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
    NewP = LocalP;
    NewZoom = Zoom;
    if(updateAgain){
      //	UpdateState();
    }
  }
