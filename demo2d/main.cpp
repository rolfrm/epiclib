#include "../Utils/ndtree.h"
#include "../Math/GenericVector.h"
#include <vector>

using namespace std;
int colorVecToInt(Vec<float,4> color){
  int out = 0;
  unsigned char * intPtr = (unsigned char *) &out;
  for(int i = 0; i < 4;i++){
    intPtr[i] = (unsigned char)(color[i] * 255.0f);
  }
  return out;
}

Vec<float,4> intToColor(int in){
  unsigned char * intPtr = (unsigned char *) &in;
  return vec(((float)intPtr[0])/255.0f,((float)intPtr[1])/255.0f,((float)intPtr[2])/255.0f,((float)intPtr[3])/255.0f);
}

using QuadTree = Node<Vec<float,4> ,2>;

int writeTreeToVector(vector<int> &  data, int ptr, QuadTree * origin){
#define set(x,y) if(data.size() <= x){data.resize(x+1);}data[x] = y;
  std::cout << "Subptr: " << ptr << "\n";
  unsigned int type = 0;
  unsigned int children = 0;
  unsigned int hasMean = 0;
  int childit = 0;
  Vec<float,4> meanColor = vec(0.0f,0.0f,0.0f,0.0f);
  float subColors = 0.0f;
  int childData = 0;
  int endPos = ptr + 2;

  for(int i = 0; i < 4;i++){
    QuadTree * child = origin->children[i];
    
    if(child == NULL){
      continue;
    }
    endPos += 1;
    if(child->Data[3] > 0){
      meanColor = meanColor + child->Data;
      subColors += 1.0f;
      hasMean = 1;
    }
    unsigned char idxMask = 1 << i;
    children |= idxMask;
    int cellv = 0;
    if(child->HasChildren()){
      
      type |= idxMask;
      cellv = -1;
    }else{
      cellv = colorVecToInt(child->Data);
      
    }
    set(ptr+2+childit,cellv);
    childit +=1;
  }
    
  for(int i = 0; i < 4;i++){
    QuadTree * child = origin->children[i];
    
    if(child == NULL){
      continue;
    }
    if(child->HasChildren()){
      std::cout << "lol?" << ptr + 2 + childit << " " << endPos-ptr << "\n";
      set(ptr +2 + childit, endPos - ptr);
      endPos = writeTreeToVector(data,endPos,child);
    }
    childit +=1;
  }

  set(ptr,(type | (children << 8) | (hasMean << 16)));
  if(subColors > 0){
    set(ptr+1,colorVecToInt(meanColor / subColors));
  }else{
    set(ptr+1,0);
  }
  return 3;
}

void renderQuadTree(QuadTree * qt){

}


int main(){
  QuadTree * qt = new QuadTree();
  qt->get_child(0,true)->Data = vec(1.0f,0.0f,0.0f,0.0f);
  qt->get_child(0,true)->get_child(0,true)->Data = vec(1.0f,1.0f,0.0f,0.0f);

  qt->get_child(1,true)->Data = vec(1.0f,0.0f,0.0f,0.0f);
  qt->get_child(2,true)->Data = vec(1.0f,0.0f,0.0f,0.0f);
  qt->get_child(3,true)->Data = vec(1.0f,0.0f,0.0f,0.0f);
  

  vector<int> data;
  writeTreeToVector(data,0,qt);
  std::cout << "Printing data: \n";
  for(int i = 0; i < data.size();i++){
    std::cout << data[i] << "\n";
  }
  std::cout << "\n" << data.size() << "\n";
}
