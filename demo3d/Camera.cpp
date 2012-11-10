#include "Camera.h"
#include "../Graphics/GraphicsCore.hpp"
#include "../Utils/Debug.h"
Camera::Camera(Vec<float,3> _rotation, Vec<float,3> _position){
  rotation = _rotation;
  position = _position;
}

Camera::Camera(){
  rotation = vec<float>(0.0,0.0,0.0);
  position = vec<float>(0.0,0.0,0.0);
}

Camera Camera::rotate(Vec<float,3> delta_rotation){
  return Camera(rotation + delta_rotation, position);
}

Camera Camera::SetRotation(Vec<float,3> rot){
  
  return Camera(rot,position);
}

Camera Camera::move(Vec<float,3> delta_position){
  return Camera(rotation,position + delta_position);
}

Camera Camera::moveRelative(Vec<float,3> dir){
    Matrix<float,4> r = RMatrix(rotation * -1);
    Vec<float,4> ndir = r * vec(dir[0],dir[1],dir[2],1.0f);
    return Camera(rotation, position + vec(ndir[0],ndir[1],ndir[2]));
  }

Matrix<float,4> Camera::getTransformMatrix(){
  Matrix<float,4> out = RTMatrix(rotation * -1, position * -1);
  
  return out;
}
