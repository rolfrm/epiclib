#include<vector>

template<class T>
std::vector<char> Serialize(T object){
  std::vector<char> out;
  char * ptr =(char *) &object;
  for(int i = 0; i < sizeof(T);i++){
    out.push_back(*ptr);
    ptr++;
  }
  return out;
}

template<typename T>
T Deserialize(std::vector<char> data){
  return *((T *) &data[0]);

}
