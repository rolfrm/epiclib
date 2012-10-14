#include "GenericVector.hpp"
#include<iostream>

template<class T>
class MatrixBase{
 public:
  int nRows;
  int nCols;
  T * Data;
  MatrixBase(int _nRows,int _nCols, T * data){
    nRows = _nRows;
    nCols = _nCols;
    Data = data;
  }
};
/*
template<class T, int rows, int cols>
  class Matrix: public MatrixBase<T>{
  Vec<T,cols> data[rows];
 public:
 Matrix():
  MatrixBase(rows,cols,&data[0]){
    
  }
  
  T & operator[](int i){
    return data[i];
  }

  };*/

template<class T, int size>
  class Matrix: public MatrixBase<T>{
  Vec<T,size> data[size];
 public:
 Matrix():MatrixBase<T>(size,size,&data[0][0])
    {

    }
 Matrix(T * _data):MatrixBase<T>(size,size,&data[0][0])
    {
      std::copy(_data,_data+size*size,&data[0][0]);
    }

  Vec<T,size> & operator[](int i){
    return data[i];
  }
  Matrix<T,size> operator*(Matrix<T,size>& other){
    Matrix<T,size> out;
    for(int i = 0;i <size;i++){
      for(int j = 0; j < size;j++){
	out[i][j] = 0;
	for(int k = 0; k < 4;k++){
	  out[i][j]+= operator[](k)[j]*other[i][k];
	}
      }
    }
    return out;
  }
  void print(){
     for(int i = 0;i <size;i++){
      for(int j = 0; j < size;j++){
	std::cout << (*this)[i][j] << " ";
      }
      std::cout << "\n";
     }
  }

};


struct matrix4x4{
  Vec<float,4> rows[4];
};

matrix4x4 mult(matrix4x4 a, matrix4x4 b){
  matrix4x4 out;
  for(int i = 0;i <4;i++){
    for(int j = 0; j < 4;j++){
      out.rows[i][j] = 0;
      for(int k = 0; k < 4;k++){
	out.rows[i][j]+= a.rows[k][i]*b.rows[j][k];
      }
    }
  }
  return out;

}

void printMatrix(matrix4x4 mat){
  for(int i = 0; i < 4;i++){
    for(int j = 0; j < 4;j++){
      std::cout << mat.rows[i][j] << " ";
    }
    std::cout << "\n";
  }
}
