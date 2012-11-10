#pragma once
#include "Camera.h"
#include "../Utils/Timing.h"
#include "../Events/input_events.h"
class CameraControl: public EventListener<mouse_position>, public EventListener<KeyEvent>{
public:

  float forward;
  float backward;
  float left;
  float right;  
  StopWatch swatch;

 public:
  Camera cam;

  CameraControl();
  Camera GetCamera();
  bool handle_event(mouse_position npos);
  bool handle_event(KeyEvent ke);
  void SetState(Vec<float,3> pos, Vec<float,3> rot);

};
