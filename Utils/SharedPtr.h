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
  SharedPtr(T _ref){
    ptr = _ref;
    ref = new int(1);
  }
  
  SharedPtr(){    
    ptr = NULL;
    ref = NULL;
  }

  T operator*(){
    if(ref == NULL || *ref == 0){
      return NULL;
    }
    return ptr;
  }

  WeakRef<T> AsWeakRef(){
    return WeakRef<T>(ptr,ref); 
  }
  SharedPtr<T> operator=(const SharedPtr<T> other){ 
   countDown();
    ptr = other.ptr;
    ref = other.ref;
    *ref += 1;
    return *this;
  }

  SharedPtr<T> operator=(T ptr){
    countDown();
    ptr = ptr;
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
      delete ptr;
    }
  }
  
  virtual ~SharedPtr(){
    countDown();    
  }  
};
