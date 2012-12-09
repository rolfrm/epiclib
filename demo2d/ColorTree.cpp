#include "ColorTree.h"
#include <map>
#include <list>
using namespace std;
void ColorTree::PostProcessChild(ColorTree * child){
    child->Data = child->parent->Data;
  }
void ColorTree::PostProcessParent(ColorTree * parent, ColorTree * madeFrom){
    //parent->Data = madeFrom->Data;
  }

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
