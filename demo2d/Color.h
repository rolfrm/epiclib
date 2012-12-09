#pragma once
#include "epiclib3/Math/GenericVector.h"
#define Color Vec<unsigned char,4>
Color color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
Color color(Vec<float,4> f);
Color col(Vec<unsigned char,4> colors);
unsigned int colorVecToUint(Color color);
Color UintToColorVec(unsigned int coli);
