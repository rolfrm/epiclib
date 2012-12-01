/*
In this module a fast,generic container for ND maps(by trees) is implemented.  

TODO: Avoid copy paste-code, but almost done

*/

#ifndef NULL
#define NULL 0
#endif
#include "../Math/GenericVector.h"
template<int N>
using IVec = Vec<int,N>;

#include <iostream>


template<class T, int D>
class Node{
  
  void make_parent(){
    
    parent = CreateNode();
    parent->idx = (idx + 1) % (1 << D);
    parent->children[idx] = this;
    
  }

public:
  T Data;
  Node * parent;
  Node * children[1 << D];
  int idx;

  Node * CreateNode(){
    return new Node();
  }
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
      IVec<D> cp = p;
      for(int i = 0; i < 3;i++){
	cp[i] = cp[i] >> 1;
      }
      	    
      container = parent->relative_node(cp,create);
      
      if(container == NULL){
	return NULL;
      }
      for(int i = 0; i < 3;i++){
	p[i] = p[i] & 1;
      }

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

  virtual Node * make_child(int nr){
    Node * new_child = CreateNode();
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
  bool HasChildren(){
    for(int i = 0; i < (1 << D);i++){
      if(children[i] != NULL){
	return true;
      }
    }
    return false;
  }

};



template<class T, int D, class SelfType>
class VNode{
  
  void make_parent(){
    
    parent = CreateNode();
    parent->idx = (idx + 1) % (1 << D);
    parent->children[idx] = (SelfType *)this;
    
  }

public:
  T Data;
  SelfType * parent;
  SelfType * children[1 << D];
  int idx;

  virtual SelfType * CreateNode() = 0;
  VNode(){
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

  SelfType * relative_node(IVec<D> p, bool create){
    p = p + index_to_ivec(idx);
    

    SelfType * container = get_parent(create);
    if(container == NULL){
      return NULL;
    }

    if(outside_range(p)){
      IVec<D> cp = p;
      for(int i = 0; i < 3;i++){
	cp[i] = cp[i] >> 1;
      }
      	    
      container = parent->relative_node(cp,create);
      
      if(container == NULL){
	return NULL;
      }
      for(int i = 0; i < 3;i++){
	p[i] = p[i] & 1;
      }

    }
    
    int index = ivec_to_index(p);
    if(container->children[index] == NULL){
      if( create == false){
	return NULL;
      }
      SelfType * new_child = container->CreateNode();
      new_child->idx = index;
      new_child->parent = container;
      container->children[index] = new_child;
    }

    SelfType * child = container->children[index];
    
    return child;
  }
  SelfType * get_parent(bool create){
    if(parent == NULL){
      if(create == false){
	return NULL;
      }
      make_parent();
    }
    return parent;
    
  }

  virtual SelfType * make_child(int nr){
    SelfType * new_child = CreateNode();
    new_child->idx = nr;
    new_child->parent = (SelfType *)this;
    children[nr] = new_child;
    return new_child;
  }

  SelfType * get_child(int nr, bool create){
    if(children[nr] == NULL && create){
      return make_child(nr);
    }
    return children[nr];
  }

  SelfType * get_child(IVec<D> p,bool create){
    
    int index = ivec_to_index(p);
    //std::cout << index << "\n";
    return get_child(index,create);
  }
  bool HasChildren(){
    for(int i = 0; i < (1 << D);i++){
      if(children[i] != NULL){
	return true;
      }
    }
    return false;
  }

};

