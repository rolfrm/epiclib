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
template<class T, int size>
  class Matrix: public MatrixBase<T>{
  Vec<T,size> cols[size];
 public:
 Matrix():MatrixBase<T>(size,size,&data[0][0])
    {

    }
 Matrix(T * _data):MatrixBase<T>(size,size,&data[0][0])
    {
      std::copy(_data,_data+size*size,&cols[0][0]);
    }

  Vec<T,size> & operator[](int col){
    return data[col];
  }

  T & operator[](int col, int row){
    return data[col][row];
  }

  Matrix<T,size> operator*(Matrix<T,size>& other){
    Matrix<T,size> out;
    for(int i = 0;i <size;i++){
      for(int j = 0; j < size;j++){
	out[i][j] = operator[](0,i)*other[j,0];
	for(int k = 1; k < size;k++){
	  out[i][j] += operator[](k,i)*other[j,k];
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
