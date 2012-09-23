#include "quadtree.h"

int QuadTreeDemo(){
  QNode origin;
  int sx = 10;
  int sy = 10;
  QNode * nd = origin.relative_node(vec2(0,0),true);
  nd->Data = 5;
  for(int i = -sx; i < sx;i++){
    for(int j = -sy; j < sy;j++){
     
      origin.relative_node(vec2(i,j),true)->Data = i + j*10;
    }
  }

  for(int i = -sx; i < sx;i++){
    for(int j = -sy; j < sy;j++){
      if(!origin.relative_node(vec2(i,j),true)->Data == i + j*10){
	throw "quadtree error..";
      }
    }
    
  }
  return 0;
}
