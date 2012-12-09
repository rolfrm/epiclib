#pragma once
#include "epiclib3/Math/GenericVector.h"
#include "epiclib3/Graphics/Texture.hpp"
#include "epiclib3/Graphics/VertexBufferObject.hpp"
#include "epiclib3/Utils/semi_virtual_ndtree.h"
#include <map>
#include "Texture2DShader.h"
#include "ColorTree.h"
unsigned int median4(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
#define TextureNode Node<Texture,2>
class QuadtreeRenderer{
public:
  QuadTree * origin;
  Vec<double,2> LocalP; // Screen upper left corner
  Vec<double,2> NewP;
  double Zoom;
  double NewZoom;
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
  std::map<TextureNode *, int> texturesInUse;
  std::map<TextureNode *, bool> toRelease;
  std::map<QuadTree * ,bool> paintedNodes;

  QuadtreeRenderer(QuadTree * _origin,VBO squareBuffer, Texture2DShader tex2DShader, Vec<int,2> screenSize);

  void Render();
  void _repaint_down(TextureNode * cell);
  void Repaint(TextureNode * cell);

  void finishedPainting();
  Color sampleColor(Vec<int,2> point);
  void PaintDotScreen(Vec<int,2> point,Color color, int pxsize);
  void ZoomIn(double Change, Vec<double,2> Around);
  void optimizeTree(QuadTree * cell);
  Vec<double,2> ScreenToWorld(Vec<int,2> screenPos);
  void ZoomInAroundScreenPos(double Change, Vec<int,2> pos);
  void Move(Vec<double,2> amount);
  QuadTree * TraceDown(Vec<double,2> position);
  void SetWindowSize(Vec<int,2> size);
  void UpdateState();
};
