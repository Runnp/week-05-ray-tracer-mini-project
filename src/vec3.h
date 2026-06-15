#pragma once
#include <cmath>
#include <iostream>

struct Vec3 {
    double x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}


    Vec3 operator+(const Vec3& b) const { return {x+b.x, y+b.y, z+b.z}; }
    Vec3 operator-(const Vec3& b) const { return {x-b.x, y-b.y, z-b.z}; }
    Vec3 operator*(double t)      const { return {x*t,   y*t,   z*t};   }
    Vec3 operator/(double t)      const { return {x/t,   y/t,   z/t};   }

    
    Vec3 operator*(const Vec3& b) const { return {x*b.x, y*b.y, z*b.z}; }

    Vec3 operator-() const { return {-x, -y, -z}; }

    
    double dot(const Vec3& b)  const { return x*b.x + y*b.y + z*b.z; }
    double lengthSq()          const { return x*x + y*y + z*z; }
    double length()            const { return std::sqrt(lengthSq()); }
    Vec3   normalize()         const { return *this / length(); }

    
    double r() const { return x; }
    double g() const { return y; }
    double b() const { return z; }
};

inline Vec3 operator*(double t, const Vec3& v) { return v * t; }