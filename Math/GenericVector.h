
#pragma once
#include <cmath>

template<class T, int N>
class Vec{
public:
  T data[N];
  T & operator[](unsigned int i){
    return data[i];
  }
  Vec(){
    for(int i = 0; i < N;i++){
      data[i] = 0;
    }
  }
  
  template<typename T2>
    Vec<T2,N> As(){
    Vec<T2,N> out;
    for(int i = 0; i < N;i++){
      out[i] = (T2) data[i];
    }
    return out;
  }

  Vec<T,N> operator -(){
    Vec<T,N> temp;
    for(int i=0;i<N;i++)
      temp[i]= -data[i];
    return temp;
  }

#define other_vector_operator(opr) \
  Vec<T,N> operator opr (Vec<T,N> other){	\
    Vec<T,N> out;\
    for(int i = 0; i< N;i++){\
      out.data[i] = data[i] opr other.data[i];\
    }\
    return out;\
  }

#define scalar_operator(opr) \
  Vec<T,N> operator opr (T other){	\
    Vec<T,N> out;\
    for(int i = 0; i< N;i++){\
      out.data[i] =data[i] opr other;\
    }\
    return out;\
  }

  bool operator==(Vec other){
    for(int i = 0; i < N;i++){
      if(other[i] != data[i]){
	return false;
      }
    }
    return true;
  }

  static T dot(Vec v1, Vec v2){
    T out = 0;
    for(int i = 0; i < N;i++){
      out += v1.data[i]*v2.data[i];
    }
    return out;
  }



  other_vector_operator(+);
  other_vector_operator(-);
  other_vector_operator(/);
  other_vector_operator(%);
  other_vector_operator(*);

  scalar_operator(+);
  scalar_operator(-);
  scalar_operator(/);
  scalar_operator(%);
  scalar_operator(*);
  scalar_operator(>>);
  scalar_operator(<<);
  scalar_operator(&);
};

template<class T>
Vec<T,1> vec(T d0){
  Vec<T,1> out;
  out.data[0] = d0;
  return out;
}

template<class T>
Vec<T,2> vec(T d0, T d1){
  Vec<T,2> out;
  out.data[0] = d0;
  out.data[1] = d1;
  return out;
}
template<class T>
Vec<T,3> vec(T d0, T d1, T d2){
  Vec<T,3> out;
  out.data[0] = d0;
  out.data[1] = d1;
  out.data[2] = d2;
  return out;
}
template<class T>
Vec<T,4> vec(T d0, T d1, T d2, T d3){
  Vec<T,4> out;
  out.data[0] = d0;
  out.data[1] = d1;
  out.data[2] = d2;
  out.data[3] = d3;
  return out;
}

template<class T, int d>
T VectorLengthSquared(Vec<T,d> inVec){
  T out = 0;
  for(int i = 0; i < d; i++){
    double val = inVec[i];
    out += val * val;
  }
  return out;
}

template<class T, int d>
T VectorLength(Vec<T,d> inVec){
  return std::sqrt(VectorLengthSquared(inVec));
}

template<class T, int d>
  Vec<T,d> Floor(Vec<T,d> inVec){
  for(int i = 0; i < d;i++){
    inVec[i] = floor(inVec[i]);
  }
  return inVec;
}



typedef Vec<double,3> Vec3d;
typedef Vec<double,2> vec2d;

