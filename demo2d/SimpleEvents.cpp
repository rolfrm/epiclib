#include "SimpleEvents.h"
SimpleEvents::SimpleEvents(QuadtreeRenderer * qtRenderer,Texture2DShader shader, VBO squareBuffer):
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

bool SimpleEvents::handle_event(KeyEvent kev){
  std::cout << kev.key << "\n";
  if(kev.key == ESC || kev.key == 'Q'){
    Running = false;
  }
  if(kev.key == CTRL){
    ctrlDown = kev.pressed;
  }
}
bool SimpleEvents::handle_event(MouseClick mclick){
  if(mclick.button == 0){
    leftDown = mclick.pressed;
    first = true;
    if(mclick.pressed){
      if(!KeyIsDown(CTRL)){
	
	PaintAt(last[0],last[1]);
      }else{
	Color col = renderer->sampleColor(last);
	SetColor(col[0], col[1],col[2],col[3]);
      }
    }else{
      PaintDone();
    }
  }
  if(mclick.button == 1){
    rightDown = mclick.pressed;
    
  }
}
bool SimpleEvents::handle_event(MouseWheelEvent mwheel){
  double zoomAmount = 1.05;
  Zoom(mwheel.change > 0? zoomAmount : 1.0 / zoomAmount,last[0],last[1]);
  //renderer->ZoomInAroundScreenPos(mwheel.change > 0? zoomAmount : 1.0 / zoomAmount,last);
}
bool SimpleEvents::handle_event(mouse_position mpos){
  Vec<int,2> pos = vec(-mpos.x,mpos.y);
  Vec<int,2> dp = pos - last;
  if(leftDown){  
    if(!KeyIsDown(CTRL)){

      PaintAt(last[0],last[1]);
    }else{
      Color col = renderer->sampleColor(last);
      SetColor(col[0], col[1],col[2],col[3]);
    }
  }

  if(rightDown){
    renderer->Move(dp.As<double>());
  }
    
  last = pos;
  return true;
}
bool SimpleEvents::handle_event(size s){
  SetScreenSize(s.x,s.y);
}

  
void SimpleEvents::RenderBrushPreview(){
  /*texShader.SetUVState(vec(0.0,0.0),vec(1.0,1.0));
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
  texShader.SetColor(color(255,255,255,255));*/
}
