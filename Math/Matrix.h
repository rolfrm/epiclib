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

/*template<class T, int rows, int cols>
  class Matrix: public MatrixBase<T>{
  Vec<T,cols> data[rows];
 public:
 Matrix():
  MatrixBase(rows,cols,&data[0]){
    
  }
  
  T & operator[](int i){
    return data[i];
  }

  };
*/


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

  Matrix<T,size> operator*(Matrix<T,size>& other){
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

  Matrix<T,size> operator*(Vec<T,size>& other){
    Vec<T,size> out;
    for(int i = 0;i <size;i++){
      out[i] = 0;
      for(int j = 0; j < size;j++){
	out[i] += operator[](j)[i]*other[i];
      }
    }
    return out;
  }

  T * asPtr(){
    return &(operator[](0)[0]);
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
template<class T, int size>
  nullValue<T> Matrix<T,size>::nullValues[size*size];
