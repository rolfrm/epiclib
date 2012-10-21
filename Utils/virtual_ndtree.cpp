#include "virtual_ndtree.h"
IVec<3> ivec(int x, int y, int z){
  return IVec<3>{x,y,z};
}
IVec<2> ivec(int x, int y){
  return IVec<2>{x,y};
}
IVec<4> ivec(int x, int y , int z, int w){
  return IVec<4>{x,y,z,w};
}
IVec<1> ivec(int x){
  return IVec<1>{x};
}
