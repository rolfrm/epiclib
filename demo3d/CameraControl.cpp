#include "CameraControl.h"

CameraControl::CameraControl(){
    
  forward = 0.0;
  backward = 0.0;
  left = 0.0;
  right = 0.0;
}

Camera CameraControl::GetCamera(){
  double sec = swatch.ElapsedSeconds();
  Vec<float,3> d = vec(right - left,0.0f,forward-backward);
  cam = cam.moveRelative(d * sec * 10 );
  swatch.Reset();
  swatch.Start();
  return cam;
}

bool CameraControl::handle_event(mouse_position npos){
  int dx = npos.x - 200;
  int dy = npos.y - 200;
  SetMousePosition(200,200);
    
  cam = cam.rotate(vec((float)dy / 1000.0f,(float)dx / 1000.0f, 0.0f));
  return true;
}

bool CameraControl::handle_event(KeyEvent ke){
  switch(ke.key){
  case ARROW_UP:forward = (ke.pressed ? 1 : 0); break;
  case ARROW_DOWN:backward = (ke.pressed ? 1 : 0); break;
  case ARROW_LEFT:left = (ke.pressed ? 1 : 0); break;
  case ARROW_RIGHT:right = (ke.pressed ? 1 : 0); break;
  }
  return true;
}

void CameraControl::SetState(Vec<float,3> pos, Vec<float,3> rot){
  cam.rotation = rot;
  cam.position = pos;
}
