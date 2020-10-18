// operations on matrices 
#include <cstdio>
#include <cmath>
#include <iostream>

#define PI 3.1416

vec3f normalize(const vec3f& v) {
  float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  bool lengthIsZero = length < 0.00001;
  vec3f normal(lengthIsZero ? 0 : v.x / length,
               lengthIsZero ? 0 : v.y / length,
               lengthIsZero ? 0 : v.z / length);
  return normal;
}

vec3f cross(const vec3f& a, const vec3f& b) {
  vec3f v(a.y * b.z - a.z * b.y,
          a.z * b.x - a.x * b.z,
          a.x * b.y - a.y * b.x);
  return v;
}

vec3f subtract(const vec3f& a, const vec3f& b) {
  vec3f v(a.x - b.x, a.y - b.y, a.z - b.z);
  return v;
}

matrix4f scale(const float a) {
  matrix4f s(a, 0, 0, 0,
             0, a, 0, 0, 
             0, 0, a, 0,
             0, 0, 0, a);
  return s;
}

matrix4f multiply(const matrix4f& a, const matrix4f& b) {
  matrix4f c;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      c[i][j] = a[i][0] * b[0][j] +
                a[i][1] * b[1][j] +
                a[i][2] * b[2][j] +
                a[i][3] * b[3][j];
    }
  }
  return c;
}

vec3f multiply(const matrix4f& m, const vec3f& v) {
  float a = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + m[3][0];
  float b = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + m[3][1];
  float c = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + m[3][2];
  float w = v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[2][3] + m[3][3];
  vec3f r(a / w, b / w, c / w);

  return r;
}

matrix4f invert( matrix4f& t) {
  int i, j, k;
  matrix4f s;
        
  // Forward elimination
  for (i = 0; i < 3 ; i++) {
    int pivot = i;
    float pivotsize = t[i][i];
            
    if (pivotsize < 0)
      pivotsize = -pivotsize;
                
    for (j = i + 1; j < 4; j++) {
      float tmp = t[j][i];
                    
      if (tmp < 0)
        tmp = -tmp;
                        
      if (tmp > pivotsize) {
       pivot = j;
       pivotsize = tmp;
      }
    }
  
    if (pivotsize == 0) {
     return matrix4f(); // Cannot invert singular matrix
    }
            
    if (pivot != i) {
     for (j = 0; j < 4; j++) {
        float tmp = t[i][j];
        t[i][j] = t[pivot][j];
        t[pivot][j] = tmp;
                    
        tmp = s[i][j];
        s[i][j] = s[pivot][j];
        s[pivot][j] = tmp;
      }
    }
            
    for (j = i + 1; j < 4; j++) {
      float f = t[j][i] / t[i][i];
                
      for (k = 0; k < 4; k++) {
        t[j][k] -= f * t[i][k];
        s[j][k] -= f * s[i][k];
      }
    }
  }
        
  // Backward substitution
  for (i = 3; i >= 0; --i) {
    float f;
            
    if ((f = t[i][i]) == 0) {
      return matrix4f(); // Cannot invert singular matrix
    }
            
    for (j = 0; j < 4; j++) {
      t[i][j] /= f;
      s[i][j] /= f;
    }
            
    for (j = 0; j < i; j++) {
      f = t[j][i];
      for (k = 0; k < 4; k++) {
        t[j][k] -= f * t[i][k];
        s[j][k] -= f * s[i][k];
      }
    }
  }
  return s;
}

matrix4f computeCameraToWorldMatrix(vec3f &cameraPosition,
                                    vec3f &Target)
{
	const vec3f Up(0, 1, 0);
	vec3f zAxis = normalize(subtract(cameraPosition, Target));
	vec3f xAxis = normalize(cross(Up, zAxis));
  vec3f yAxis = normalize(cross(zAxis, xAxis));  
  matrix4f m(xAxis.x, xAxis.y, xAxis.z, 0, 
             yAxis.x, yAxis.y, yAxis.z, 0, 
             zAxis.x, zAxis.y, zAxis.z, 0, 
             cameraPosition.x, cameraPosition.y, cameraPosition.z, 1); 
  return m;
}

vec3f rotateX(const vec3f &p, const float theta)
{
  vec3f newP;
  float thetaD = theta * PI / 180.0;
  newP.x = p.x;
  newP.y = cos(thetaD) * p.y - sin(thetaD) * p.z;  
  newP.z = sin(thetaD) * p.y + cos(thetaD) * p.z;
  return newP;
}

vec3f rotateY(const vec3f &p, const float theta)
{
  vec3f newP;
  float thetaD = theta * PI / 180.0;
  newP.y = cos(thetaD) * p.x + sin(thetaD) * p.z;  
  newP.y = p.y;
  newP.z = -sin(thetaD) * p.x + cos(thetaD) * p.z;
  return newP;
}


class cameraToWorld {
  vec3f _cameraPosition;
  vec3f _target;
public:
  cameraToWorld(vec3f &cameraPosition,
                vec3f &target) :
                _cameraPosition(cameraPosition),
                _target(target) {}
  matrix4f operator() () {
    const vec3f Up(0, 1, 0);
    vec3f zAxis = normalize(subtract(_cameraPosition, _target));
    vec3f xAxis = normalize(cross(Up, zAxis));
    vec3f yAxis = normalize(cross(zAxis, xAxis));
    matrix4f m(xAxis.x, xAxis.y, xAxis.z, 0,
              yAxis.x, yAxis.y, yAxis.z, 0,
              zAxis.x, zAxis.y, zAxis.z, 0,
              _cameraPosition.x, _cameraPosition.y, _cameraPosition.z, 1);
    return m;
  }
};

class worldToPixel {
  matrix4f& _worldToCamera;
  float _canvasWidth;
  float _canvasHeight;
  int _imageWidth;
  int _imageHeight;
public:
  worldToPixel(matrix4f &worldToCamera,
               float canvasWidth,
               float canvasHeight,
               int imageWidth,
               int imageHeight) :
               _worldToCamera(worldToCamera),
               _canvasWidth(canvasWidth), 
               _canvasHeight(canvasHeight), 
               _imageWidth(imageWidth), 
               _imageHeight(imageHeight) {}
  vec2i operator() (vec3f pWorld) {
    vec3f pCamera = multiply(_worldToCamera, pWorld);
    vec2f pScreen(pCamera.x / -pCamera.z, pCamera.y / -pCamera.z);
    vec2f pNDC((pScreen.x + _canvasWidth * 0.5) / _canvasWidth,
               (pScreen.y + _canvasHeight * 0.5) / _canvasHeight);
    vec2i pRaster( (int)(pNDC.x * _imageWidth), 
                    (int)(( 1 - pNDC.y) * _imageHeight));
    return pRaster;
  }
};
