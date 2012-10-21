#pragma once

template<class T>
class RestrictedFilePtr{
 public:
  long int addr;
  RestrictedFilePtr(long int _addr){
    addr = _addr;
  }
  RestrictedFilePtr(){
    addr = -1;
  }
};

template<class T>
class FilePtr{
 public:

  long int addr;

  FilePtr(long int _addr){
    addr = _addr;
  }
  FilePtr(){
    addr = -1;
  }

};
