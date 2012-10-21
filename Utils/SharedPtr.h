

template<class T>
class SharedPtr{
  T * ptr;
  int * ref;
 public:
  SharedPtr(T * ref){
    ptr = ref;
    ref = new int(1);
  }
  
  SharedPtr(){

  }

  Sharedptr(SharedPtr<T> & other){
    countDown();
    ptr = other.ptr;
    ref = other.ref;
    *ref += 1;
  }

  void countDown(){
    *ref -= 1;
    if(*ref <= 0){
      delete ref;
      delete ptr;
    }
  }
  
  ~SharedPtr(){
    countDown();    
  }
  
  
};
