#include "../Math/GenericVector.hpp"
#include "../Math/Matrix.h"
#include "../Math/Quat.hpp"
#include "../Math/CommonMatrixOperations.hpp"

class Camera{

public:
  Vec<float,3> rotation;
  Vec<float,3> position;

  Camera(Vec<float,3> _rotation, Vec<float,3> _position);
  Camera();

  Camera rotate(Vec<float,3> delta_rotation);
  Camera SetRotation(Vec<float,3> rot);
  Camera move(Vec<float,3> delta_position);
  Matrix<float,4> getTransformMatrix();

};
