// user-defined types for primitive geometry objects (points, lines) 

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

template<typename T>
class vec2 {
public:
  T x, y;
  T v[2] = {x, y};
  vec2() : x(0), y(0) {}
  vec2(T x, T y) : x(x), y(y) {}
  friend std::ostream& operator << (std::ostream &s, const vec2<T> &v) {
    return s << '[' << v.x << ' ' << v.y << ']';
  } 
};

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;
typedef vec2i point2d; 

template<typename T>
class vec3 {
public:
  T x, y, z;
  vec3() : x(0), y(0), z(0) {} 
  vec3(T x, T y, T z) : x(x), y(y), z(z) {}
  const T& operator [] (int i) const { 
    if (i==0) return x;
    if (i==1) return y;
    if (i==2) return z;
    return x;
  }
  T& operator [] (int i) { 
    if (i==0) return x;
    if (i==1) return y;
    if (i==2) return z;
    return x;
  }
  friend std::ostream& operator << (std::ostream &s, const vec3<T> &v) {
    return s << '[' << v[0] << ' ' << v[1] << ' ' << v[2] << ']';
  } 
};

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;
typedef vec3f point3d;

template<typename T> class matrix4 {
public:
  T x[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
  matrix4() {}
  matrix4(T a, T b, T c, T d, T e, T f, T g, T h, 
          T i, T j, T k, T l, T m, T n, T o, T p) { 
    x[0][0] = a; x[0][1] = b; x[0][2] = c; x[0][3] = d;
    x[1][0] = e; x[1][1] = f; x[1][2] = g; x[1][3] = h;
    x[2][0] = i; x[2][1] = j; x[2][2] = k; x[2][3] = l;
    x[3][0] = m; x[3][1] = n; x[3][2] = o; x[3][3] = p; 
  }
  const T* operator [] (int i) const { return x[i]; }               
  T* operator [] (int i) { return x[i]; } 

  friend std::ostream& operator << (std::ostream &s, const matrix4 &m) {
    std::ios_base::fmtflags oldFlags = s.flags();
    int width = 12; // total with of the displayed number
    s.precision(5); // control the number of displayed decimals
    s.setf (std::ios_base::fixed);

        s << "[" << std::setw (width) << m[0][0] <<
             " " << std::setw (width) << m[0][1] <<
             " " << std::setw (width) << m[0][2] <<
             " " << std::setw (width) << m[0][3] << "\n" <<

             " " << std::setw (width) << m[1][0] <<
             " " << std::setw (width) << m[1][1] <<
             " " << std::setw (width) << m[1][2] <<
             " " << std::setw (width) << m[1][3] << "\n" <<

             " " << std::setw (width) << m[2][0] <<
             " " << std::setw (width) << m[2][1] <<
             " " << std::setw (width) << m[2][2] <<
             " " << std::setw (width) << m[2][3] << "\n" <<

             " " << std::setw (width) << m[3][0] <<
             " " << std::setw (width) << m[3][1] <<
             " " << std::setw (width) << m[3][2] <<
             " " << std::setw (width) << m[3][3] << "]\n";
    s.flags(oldFlags);
    return s;
  }
};

typedef matrix4<float> matrix4f;
typedef matrix4<int> matrix4i;
