/*
In this module a fast,generic container for ND maps(by trees) is implemented.  

TODO: Avoid copy paste-code, but almost done

*/

#ifndef NULL
#define NULL 0
#endif

int mod2(int x);
 
template<int N>
class IVec{
public:
  int data[N];
  IVec<N> operator/(int scale){
    IVec<N> out;
    for(int i = 0; i <N;i++){
      out.data[i] = data[i]/scale;
    }
    return out;
  }

  IVec operator+(IVec<N> other){
    IVec<N> out;
    for(int i = 0; i< N;i++){
      out.data[i] = other.data[i] + data[i];
    }
    return out;
  }

  IVec operator>>(int bits){
    IVec<N> out;
    for(int i = 0; i < N ; i++){
      out.data[i] = data[i] >> bits;
    }
    return out;
  }

  IVec operator&(int bits){
    IVec<N> out;
    for(int i = 0; i < N ; i++){
      out.data[i] = data[i] & bits;
    }
    return out;
  }


};
#include <iostream>
template<class T, int D>
class Node{
  
  void make_parent(){
    
    parent = new Node();
    parent->idx = (idx + 1) % (1 << D);
    parent->children[idx] = this;
    
  }

public:
  T Data;
  Node * parent;
  Node * children[1 << D];
  int idx;


  Node(){
    parent = NULL;
    for(int i = 0; i < (1 << D);i++){
      children[i] = NULL;
    }
    idx = 0;
  }

  static IVec<D> index_to_ivec(int index){
    IVec<D> out;
    for(int i = 0; i < D;i++){
      out.data[i] = (index & (1 << i)) >> i;
    }
    return out;
  }

  static int ivec_to_index(IVec<D> in){
    int o = 0;
    for(int i = 0; i < D; i++){
      o += in.data[i] * (1 << i); 
    }
    return o;
  }

  bool outside_range(IVec<D> in){
    
    for(int i = 0; i < D;i++){
      if(in.data[i] < 0 || in.data[i] > 1){
	return true;
      }
    }
    return false;
  }

  Node * relative_node(IVec<D> p, bool create){
    p = p + index_to_ivec(idx);
    

    Node * container = get_parent(create);
    if(container == NULL){
      return NULL;
    }

    if(outside_range(p)){
      IVec<D> cp = p >> 1;
      	    
      container = parent->relative_node(cp,create);
      
      if(container == NULL){
	return NULL;
      }
      p = p & 1;

    }
    
    int index = ivec_to_index(p);
    if(container->children[index] == NULL){
      if( create == false){
	return NULL;
      }
      Node * new_child = new Node();
      new_child->idx = index;
      new_child->parent = container;
      container->children[index] = new_child;
    }

    Node * child = container->children[index];
    
    return child;
  }
  Node * get_parent(bool create){
    if(parent == NULL){
      if(create == false){
	return NULL;
      }
      make_parent();
    }
    return parent;
    
  }

  Node * make_child(int nr){
     Node * new_child = new Node();
      new_child->idx = nr;
      new_child->parent = this;
      children[nr] = new_child;
      return new_child;
  }

  Node * get_child(int nr, bool create){
    if(children[nr] == NULL && create){
      return make_child(nr);
    }
    return children[nr];
  }
  Node * get_child(IVec<D> p,bool create){
    
    int index = ivec_to_index(p);
    //std::cout << index << "\n";
    return get_child(index,create);
  }

};

IVec<3> make_vec3(int x, int y, int z);
IVec<2> make_vec2(int x, int y);
IVec<4> make_vec4(int x, int y , int z, int w);
IVec<1> make_vec1(int x);
