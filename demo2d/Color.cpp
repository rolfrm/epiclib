#include "Color.h"
Color color(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
  return vec( r ,g, b,  a);
}

Color color(Vec<float,4> f){
  return (f * 255.0f).As<unsigned char>();
}

Color col(Vec<unsigned char,4> colors){
  return color(colors[0],colors[1],colors[2],colors[3]);
}

unsigned int colorVecToUint(Color color){
  unsigned int colori = 0;
  for(int i = 0; i < 4;i++){
    colori |= ((unsigned int)(color[i]) << (i*8));
  }
  return colori;
}

Color UintToColorVec(unsigned int coli){
  Color * c=(Color *) &coli;
  return *c;
}
