#ifndef MATH_H
#define MATH_H
#include <cmath>
#include <vector>
#include <iostream>

#define ROUND(x) ((int) (x + 0.5))

class Vec3 {
public: // data
    float x,y,z;

public: // constructor
    Vec3(float x=0, float y=0, float z=0): x(x),y(y),z(z) {}
    Vec3(std::initializer_list<float> data);

public: // operators
    friend std::ostream& operator<<(std::ostream&, Vec3);
    friend Vec3 operator+(Vec3 u, Vec3 v) { return Vec3(u.x+v.x, u.y+v.y, u.z+v.z); }
    friend Vec3 operator-(Vec3 u, Vec3 v) { return Vec3(u.x-v.x, u.y-v.y, u.z-v.z); }
    friend Vec3 operator*(Vec3 u, float d) { return Vec3(u.x*d, u.y*d, u.z*d); }
    friend Vec3 operator*(float d, Vec3 u) { return u*d; }
    friend Vec3 operator/(Vec3 u, float d) { return Vec3(u.x/d, u.y/d, u.z/d); }
    float& operator[](int i) { return (&x)[i]; }

public: // operations
    friend float dot(Vec3 u, Vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
    friend Vec3 cross(Vec3 u, Vec3 v) { return
        Vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
    float norm() const { return x*x + y*y + z*z; }
    float mag() const { return sqrt(norm()); }
    Vec3 normalize() const { return (*this)/mag(); }
};

class Vec2 : public Vec3 {
public: // data
    Vec3 c; // color
    float i; // intensity

public: // constructors
    Vec2(float x=0, float y=0, float z=0, float i=1): Vec3(x,y,z),i(i) {}
};

class Mat {
public: // data
    int row, col;
    std::vector<float> data;

public: // constructor
    Mat(int r, int c, float d=0): row(r), col(c), data(r*c) { for (auto& e: data) e = d; }
    Mat& set(std::initializer_list<float> _data);

public: // operator
    friend std::ostream& operator<<(std::ostream&, Mat);
    float& operator()(int r, int c) { return data[r*col + c]; }
    float operator()(int r, int c) const { return data[r*col + c]; }
    float& operator()(int i) { return data[i]; }
    float operator()(int i) const { return data[i]; }
    friend Mat operator+(Mat A, Mat B);
    friend Mat operator-(Mat A, Mat B);
    friend Mat operator*(Mat A, Mat B);
    friend Mat operator*(Mat A, float d);
    friend Mat operator/(Mat A, float d);
    friend Mat operator*(float d, Mat A) { return A*d; }

public: // misc
    friend float dot(Mat A, Mat B);
    float mag() const;
};

#endif // MATH_H
