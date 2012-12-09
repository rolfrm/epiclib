#pragma once
#include "Color.h"
#include "epiclib3/Utils/semi_virtual_ndtree.h"
#include<istream>
#include<ostream>
class ColorTree: 
  public VNode<Color,2,ColorTree> {
public:
  
    static void PostProcessChild(ColorTree * child);
    static void PostProcessParent(ColorTree * parent, ColorTree * madeFrom);
};

#define QuadTree ColorTree
#define QuadTreeScale 8
#define QuadTreeSize (1 << QuadTreeScale)
bool RenderQuadToImage(QuadTree * qt, unsigned int * bytes, int width, int height, Vec<int,2> p, int s);
bool RenderQuadtreeToImage(QuadTree * qt, unsigned int * bytes, int outwidth, int outheight, Vec<int,2> offset);
void Serialize(QuadTree * qt, std::ostream & ostr);
QuadTree * Deserialize(std::istream & istr);
void writeQuadTreeState(QuadTree * qt, std::ostream & str);
QuadTree * loadQuadtreeState(std::istream & str);
