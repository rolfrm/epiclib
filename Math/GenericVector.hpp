
template<class T, int N>
class Vec{
public:
  T data[N];

#define op1(opr) \
  Vec<T,N> operator opr (Vec<T,N> other){	\
    Vec<T,N> out;\
    for(int i = 0; i< N;i++){\
      out.data[i] = other.data[i] opr data[i];\
    }\
    return out;\
  }

#define op2(opr) \
  Vec<T,N> operator opr (T other){	\
    Vec<T,N> out;\
    for(int i = 0; i< N;i++){\
      out.data[i] = other.data[i] opr other;\
    }\
    return out;\
  }

  static T dot(Vec v1, Vec v2){
    T out = 0;
    for(int i = 0; i < N;i++){
      out += v1.data[i]*v2.data[i];
    }
    return out;
  }

  op1(+);
  op1(-);
  op1(/);
  op1(%);
  op1(*);

  op2(+);
  op2(-);
  op2(/);
  op2(%);
  op2(*);

};



template<class T>
Vec<T,1> Vec1(T d0){
  Vec<T,1> out;
  out.data[0] = d0;
  return out;
}

template<class T>
Vec<T,2> Vec2(T d0, T d1){
  Vec<T,2> out;
  out.data[0] = d0;
  out.data[1] = d1;
  return out;
}
template<class T>
Vec<T,3> Vec3(T d0, T d1, T d2){
  Vec<T,3> out;
  out.data[0] = d0;
  out.data[1] = d1;
  out.data[2] = d2;
  return out;
}
template<class T>
Vec<T,4> Vec4(T d0, T d1, T d2, T d3){
  Vec<T,4> out;
  out.data[0] = d0;
  out.data[1] = d1;
  out.data[2] = d2;
  out.data[3] = d3;
  return out;
}
