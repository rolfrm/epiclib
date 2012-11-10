#pragma once
#include "Matrix.h"

Matrix<float,4> SMatrix(Vec<float,3> s);
Matrix<float,4> TMatrix(Vec<float,3> t);
Matrix<float,4> RMatrix(Vec<float,3> r);
Matrix<float,4> TRSMatrix(Vec<float,3> t, Vec<float,3> r, Vec<float,3> s);
Matrix<float,4> RTMatrix(Vec<float,3> r, Vec<float,3> t);
Matrix<float,4> TRMatrix(Vec<float,3> t, Vec<float,3> r);
Matrix<float,4> ProjectionMatrix(float width, float height,float near, float far);


template<class T>
Matrix<T,4> Translate(Matrix<T,4> in, float x, float y, float z){
  in[3][0] = x;
  in[3][1] = y;
  in[3][2] = z;
  return in;
}
