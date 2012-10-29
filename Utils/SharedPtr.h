#pragma once
#ifndef NULL
#define NULL 0
#endif
#include <iostream>
template<class T>
class WeakRef{
  int * ref;
  T obj;
  
 public:
  WeakRef(T _ref,int * ref_cnt){
    obj = _ref;
    ref = ref_cnt;
  }
  
  WeakRef(){
    obj = NULL;
    ref = NULL;
  }
  
  T operator*(){
    if(ref == NULL || *ref == 0){
      return NULL;
    }
    return obj;
  }

};

template<class T>
class SharedPtr{
  
  int * ref;
  T ptr;
  
 public:
  
  int GetRefCount(){
    if(ref == NULL){
      return 0;
    }
    return *ref;
  }

  SharedPtr(T _ref){
    ptr = _ref;
    ref = new int(1);
  }
  SharedPtr(){
    ref = new int(1);
  }
  
  T & operator*(){
    return ptr;
  }

  T & Get(){
    
    return ptr;
  }

  WeakRef<T> AsWeakRef(){
    return WeakRef<T>(ptr,ref); 
  }
  virtual SharedPtr<T> operator=(const SharedPtr<T> other){ 
   countDown();
    ptr = other.ptr;
    ref = other.ref;
    *ref += 1;
    return *this;
  }

  virtual SharedPtr<T> operator=(T _ptr){
    countDown();
    ptr = _ptr;
    ref = new int(1);
    return *this;
  }


  SharedPtr(const SharedPtr<T> & other){
    ptr = other.ptr;
    ref = other.ref;
    if(ref != NULL){
      *ref += 1;
    }
  }

  void countDown(){
    if(ref == NULL){
      return;
    }
    *ref -= 1;
    if(*ref <= 0){
      delete ref;
      ptr.Dispose();
    }
  }
  
  virtual ~SharedPtr(){
    countDown();    
  }  
};

template<class T>
class SharedPtr<T *>{
  
  int * ref;
  T * ptr;
  
 public:
  
  int GetRefCount(){
    if(ref == NULL){
      return 0;
    }
    return *ref;
  }

  SharedPtr(T *_ref){
    ptr = _ref;
    ref = new int(1);
  }
  
  SharedPtr(){    
    ptr = NULL;
    ref = NULL;
  }

  T * operator*(){
    if(ref == NULL || *ref == 0){
      return NULL;
    }
    return ptr;
  }

  T * Get(){
    if(ref == NULL || *ref == 0){
      return NULL;
    }
    return ptr;
  }



  WeakRef<T *> AsWeakRef(){
    return WeakRef<T *>(ptr,ref); 
  }
  virtual SharedPtr<T *> operator=(const SharedPtr<T *> other){ 
   countDown();
    ptr = other.ptr;
    ref = other.ref;
    *ref += 1;
    return *this;
  }

  virtual SharedPtr<T *> operator=(T * _ptr){
    countDown();
    ptr = _ptr;
    ref = new int(1);
    return *this;
  }


  SharedPtr(const SharedPtr<T *> & other){
    ptr = other.ptr;
    ref = other.ref;
    if(ref != NULL){
      *ref += 1;
    }
  }

  void countDown(){
    if(ref == NULL){
      return;
    }
    *ref -= 1;
    if(*ref <= 0){
      delete ref;
      delete ptr;
    }
  }
  
  virtual ~SharedPtr(){
    countDown();    
  }  
};




template<class T>
class SharedObject{
  int * ref;
  
 public:
  SharedObject(){
    ref = new int(1);
  }
  

  /*T operator=(const T other){ 
   countDown();
    ptr = other.ptr;
    ref = other.ref;
    *ref += 1;
    return *this;
  }

  T operator=(T ptr){
    countDown();
    ref = new int(1);
    return *this;
    }
  */

  SharedObject(const T & other){
    ref = other.ref;
    if(ref != NULL){
      *ref += 1;
    }
  }

  void countDown(){
    if(ref == NULL){
      return;
    }
    *ref -= 1;
    if(*ref <= 0){
      delete ref;
      Dispose();
    }
  }

  virtual void Dispose(){

  }
  
  virtual ~SharedObject(){
    countDown();    
  }  
};
