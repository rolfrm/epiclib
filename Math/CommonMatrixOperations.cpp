#include "CommonMatrixOperations.hpp"
#include <math.h>
#include "Quat.hpp"



Matrix<float,4> RotationMatrix4x4(float theta, float phi, float gamma) {
  float sinphi = sin(phi), cosphi = cos(phi);
  float sintheta = sin(theta), costheta = cos(theta);
  float singamma = sin(gamma), cosgamma = cos(gamma);
  
  float data[] = { cosgamma * costheta, 
		   costheta * singamma,
		   -sintheta, 
		   0,
		   cosgamma * sinphi * sintheta - cosphi * singamma, 
		   cosphi * cosgamma + sinphi * sintheta * singamma, 
		   costheta * sinphi, 
		   0, 
		   cosphi * cosgamma * sintheta + sinphi * singamma, 
		   -cosgamma * sinphi + cosphi * sintheta * singamma, 
		   cosphi * costheta, 
		   0, 
		   0,
		   0,
		   0,
		   1 };
  /*
  float data[] = { cosgamma * costheta,cosgamma * sinphi * sintheta - cosphi * singamma,cosphi * cosgamma * sintheta + sinphi * singamma, 0,
		   costheta * singamma, cosphi * cosgamma + sinphi * sintheta * singamma, -cosgamma * sinphi + cosphi * sintheta * singamma,0,
		   -sintheta, costheta * sinphi, cosphi * costheta, 0,
		   0,0,0,1};
  */
  
  return Matrix<float,4>(data);
}

Matrix<float,4> RMatrix(Vec<float,3> r){
  //return RotationMatrix4x4(r[0],r[1],r[2]);
  Quaternion t1(sin(r[0] / 2), 0, 0, cos(r[0] / 2));
  Quaternion t2(0, sin(r[1] / 2), 0, cos(r[1] / 2));
  Quaternion t3(0, 0, sin(r[2] / 2), cos(r[2] / 2));
  Quaternion tot = t1 * t2 * t3;
  return tot.AsMatrix();
  
}
Matrix<float,4> TMatrix(Vec<float,3> t){
  Matrix<float,4> eye = Matrix<float,4>::Eye();
  for(int i = 0; i < 3;i++){
    eye[3][i] = t[i];
  }
  return eye;
}

Matrix<float,4> SMatrix(Vec<float,3> s){
  Matrix<float,4> eye = Matrix<float,4>::Eye();
  for(int i = 0; i < 3;i++){
    eye[i][i] = s[i];
  }
  return eye;
}


Matrix<float,4> TRSMatrix(Vec<float,3> t, Vec<float,3> r, Vec<float,3> s){
  return TMatrix(t) * RMatrix(r) * SMatrix(s);
}

Matrix<float,4> RTMatrix(Vec<float,3> r, Vec<float,3> t){
  Matrix<float,4> R = RMatrix(r);
  Matrix<float,4> T = TMatrix(t);
  return R * T;
}


Matrix<float,4> ProjectionMatrix(float width, float height,float near, float far){
  Matrix<float,4> omat = Matrix<float,4>::Zeros();

  omat[0][0] = near / width;
  omat[1][1] = near / height;
  omat[2][2] = - (near + far) / (far - near);
  omat[2][3] = -1;
  omat[3][2] = -2 * far * near / (far - near);
  return omat;
}
