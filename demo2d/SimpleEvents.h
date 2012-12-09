#pragma once
#include "Texture2DShader.h"
#include "epiclib3/Events/input_events.h"
#include "epiclib3/Graphics/VertexBufferObject.hpp"
#include "epiclib3/Math/GenericVector.h"
#include "QuadtreeRenderer.h"
#include "epiclib3/Graphics/Texture.hpp"
#include "infinidraw_interface.h"
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
  Vec<int,2> BrushPosition;
  QuadtreeRenderer * renderer;

  bool Running;
  Texture2DShader texShader;
  VBO SquareBuffer;
  Texture nullTex;

  SimpleEvents(QuadtreeRenderer * qtRenderer,Texture2DShader shader, VBO squareBuffer);
  bool handle_event(KeyEvent kev);
  bool handle_event(MouseClick mclick);
  bool handle_event(MouseWheelEvent mwheel);
  bool handle_event(mouse_position mpos);
  bool handle_event(size s);
  void RenderBrushPreview();
};
