#pragma once
#include "../Math/Matrix.h"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Graphics/Program.hpp"

class Shader3D{
public:
  
  virtual void SetModelView(Matrix<float,4> modelView) = 0;
  virtual void SetProjection(Matrix<float,4> projection) = 0;
  virtual void SetCamera(Matrix<float,4> camera) = 0;
  virtual DrawMethod GetDrawMethod() = 0;
};

class FlatShader3D: public Shader3D{
public:
  Program shader;
  Matrix<float,4> projection;
  Matrix<float,4> camera;
  Matrix<float,4> total;
  Matrix<float,4> model;
  FlatShader3D();
  void update();
  void SetModelView(Matrix<float,4> modelView);
  void SetProjection(Matrix<float,4> _projection);
  void SetCamera(Matrix<float,4> cameraMat);
  DrawMethod GetDrawMethod();
};
