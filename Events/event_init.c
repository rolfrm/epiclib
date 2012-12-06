#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glfw.h>
#include <IL/il.h>
#include "event_handling.h"
#include "input_events.h"
#include <list>

EventSpawner<KeyEvent> key_event_handler;
EventSpawner<MouseClick> mouse_click_handler;
EventSpawner<mouse_position> mouse_move_spawner;
EventSpawner<CharEvent> char_event_spawner;
EventSpawner<MouseWheelEvent> mouse_wheel_event_spawner;
EventSpawner<size> window_resize_event;

void GLFWCALL mousebuttoncallback(int button, int action){
  MouseClick mEv(button,action);
  mouse_click_handler.spawn_event(mEv);

}

void SetMousePosition(int x, int y){
  glfwSetMousePos(x,y);
}

#include <iostream>
int last_wheel_pos = 0;
void GLFWCALL mouse_wheel_callback(int mwpos){
  MouseWheelEvent mEv(mwpos, mwpos - last_wheel_pos);
  last_wheel_pos = mwpos;
  mouse_wheel_event_spawner.spawn_event(mEv);
}

void GLFWCALL keycallback( int key, int action ){
  KeyEvent newKeyEv(key,action);
  key_event_handler.spawn_event(newKeyEv);
}
#include<iostream>
void GLFWCALL charcallback( int character, int action ){
  CharEvent newKeyEv(character,action==1);
  char_event_spawner.spawn_event(newKeyEv);
}
bool block_window_sizing = true;
void GLFWCALL window_size_callback(int width,int height){
  glViewport(0,0,width,height);
  window_resize_event.spawn_event(size({width,height}));
}
mouse_position global_mpos;

void GLFWCALL mouse_move_callback(int x, int y){
  global_mpos.x = x;
  global_mpos.y = y;
  mouse_move_spawner.spawn_event(global_mpos);
}

extern "C"{

  bool KeyIsDown(int key){
    return glfwGetKey( key);
  }

void init_events()
{
  last_wheel_pos = glfwGetMouseWheel();
  glfwSetKeyCallback(keycallback);
  glfwSetMouseButtonCallback(mousebuttoncallback);
  glfwSetMousePosCallback(mouse_move_callback);
  glfwSetCharCallback(charcallback);
  glfwSetMouseWheelCallback(mouse_wheel_callback);
  glfwSetWindowSizeCallback(window_size_callback);
}


mouse_position get_mouse_position(){
  return global_mpos;
}
}
