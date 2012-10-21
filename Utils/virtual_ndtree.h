/*
In this module a fast,generic container for ND maps(by trees) is implemented.  

TODO: Avoid copy paste-code, but almost done

*/

#ifndef NULL
#define NULL 0
#endif

#include <iostream>
#include "../Math/GenericVector.h"

template<int D>
class Node{

public:
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
  virtual Node<D> * Create(){
    return new Node<D>();
  }

  static Vec<int,D> index_to_ivec(int index){
    IVec<D> out;
    for(int i = 0; i < D;i++){
      out.data[i] = (index & (1 << i)) >> i;
    }
    return out;
  }

  static int ivec_to_index(Vec<int,D> in){
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

  Node * relative_node(Vec<int,D> p){
    bool allZero = true;
    for(int i = 0; i < D;i++){
      
    }
    p = p + index_to_ivec(idx);
    
    Node * container = get_parent();
    
    if(container == NULL){
      return NULL;
    }

    if(outside_range(p)){
      Vec<int,D> cp = p >> 1;
      	    
      container = parent->relative_node(cp);
      
      if(container == NULL){
	return NULL;
      }
      p = p & 1;
    }
    
    return  container->get_child(p);
    
  }

  bool has_parent(){
    return parent == NULL;
  }

  bool has_child(int nr){
    return children[nr] != NULL;
  }

  virtual Node * get_parent(){
    return parent;    
  }

  virtual void make_child(int nr){
    Node * new_child = Create();
    new_child->idx = nr;
    new_child->parent = this;
    children[nr] = new_child;
  }

  int idxlookup[10] = {0,1,3,7,15};

  virtual void  make_parent(){
    int nidx = 0;
    if(idx != 0){
      int newIdx = idxlookup[D];
    }
    parent = Create();
    parent->idx = 0;
    idx = nidx;
    parent->children[idx] = this;
  }

  virtual Node * get_child(int nr){
    return children[nr];
  }

  Node * get_child(IVec<D> p){
    int index = ivec_to_index(p);
    return get_child(index);
  }

};
