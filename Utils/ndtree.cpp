#include "ndtree.h"
int mod2(int x){
  return x & 1;
}

IVec<3> make_vec3(int x, int y, int z){
  IVec<3> vec3;
  vec3.data[0] = x;
  vec3.data[1] = y;
  vec3.data[2] = z;
  return vec3;

}

IVec<2> make_vec2(int x, int y){
  IVec<2> vec2;
  vec2.data[0] = x;
  vec2.data[1] = y;
  return vec2;
}

IVec<4> make_vec4(int x, int y , int z, int w){
  IVec<4> vec4;
  vec4.data[0] = x;
  vec4.data[1] = y;
  vec4.data[2] = z;
  vec4.data[3] = w;
  return vec4;
}

IVec<1> make_vec1(int x){
  IVec<1> vec1;
  vec1.data[0] = x;
  return vec1;
}

int node_top = 0;
