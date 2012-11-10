#pragma once
#include "GenericVector.h"
#include<algorithm>
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
Hack to ensure initial value of 0
*/
template<class T> 
class nullValue{
 public:
  T v;
  nullValue(){
    v = 0;
  }
};

template<class T, int size>
  class Matrix: public MatrixBase<T>{
  Vec<T,size> cols[size];
  static nullValue<T> nullValues [size*size];
 public:
 Matrix():MatrixBase<T>(size,size,&cols[0][0])
    {

    }
 Matrix(T * _data):MatrixBase<T>(size,size,&cols[0][0])
    {
      std::copy(_data,_data+size*size,&cols[0][0]);
    }

  Vec<T,size> & operator[](int col){
    return cols[col];
  }

  template<typename T2>
    Matrix<T2,size> As(){
    Matrix<T2,size> out;
    for(int i = 0; i < size;i++){
      out[i] = cols[i].As<T2>();
    }
    return out;
  }


  static Matrix<T,size> Zeros(){
    Matrix<T,size> out;
    std::copy(&nullValues[0].v,((&nullValues[0].v) + size*size),&out[0][0]);
    return out;
  }

  static Matrix<T,size> Eye(){
    Matrix<T,size> out = Zeros();
    for(int i = 0; i < size;i++){
      out[i][i] = 1;
    }
    return out;
  }

  Matrix<T,size> operator*(Matrix<T,size> other){
    Matrix<T,size> out;
    for(int i = 0;i <size;i++){
      for(int j = 0; j < size;j++){
	out[j][i] = operator[](0)[i]*other[j][0];
	for(int k = 1; k < size;k++){
	  out[j][i] += operator[](k)[i]*other[j][k];
	}
	
      }
    }
    return out;
  }

  Vec<T,size> operator*(Vec<T,size> other){
    Vec<T,size> out;
    for(int i = 0;i <size;i++){
      out[i] = 0;
      for(int j = 0; j < size;j++){
	out[i] += operator[](i)[j] * other[j];
      }
    }
    return out;
  }

  T * AsPtr(){
    return &(operator[](0)[0]);
  }
  
  Matrix<T,size> transpose(){
  	Matrix<T,size> trans;
  	for(int i=0;i<size;i++){
  		for(int j=0;j<size;j++){
  			trans[i][j]=cols[j][i];
  		}	
  	}
  	return trans;	
  }


};

template<class T, int size>
  nullValue<T> Matrix<T,size>::nullValues[size*size];
