#pragma once
#include <cmath>
#include <cstdlib>

struct Vec3 {
    double x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& b) const { return {x+b.x, y+b.y, z+b.z}; }
    Vec3 operator-(const Vec3& b) const { return {x-b.x, y-b.y, z-b.z}; }
    Vec3 operator*(double t)      const { return {x*t,   y*t,   z*t};   }
    Vec3 operator/(double t)      const { return {x/t,   y/t,   z/t};   }
    Vec3 operator*(const Vec3& b) const { return {x*b.x, y*b.y, z*b.z}; }
    Vec3 operator+(double t)      const { return {x+t,   y+t,   z+t};   }
    Vec3 operator-()              const { return {-x,    -y,    -z};     }

    double dot(const Vec3& b) const { return x*b.x + y*b.y + z*b.z; }
    double lengthSq()         const { return x*x + y*y + z*z; }
    double length()           const { return std::sqrt(lengthSq()); }
    Vec3   normalize()        const { return *this / length(); }

    double r() const { return x; }
    double g() const { return y; }
    double b() const { return z; }
};

inline Vec3 operator*(double t, const Vec3& v) { return v * t; }

inline double randomDouble() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline Vec3 randomVec3() {
    return Vec3(randomDouble(), randomDouble(), randomDouble());
}

inline Vec3 randomInUnitSphere() {
    while (true) {
        Vec3 p = randomVec3() * 2.0 - Vec3(1,1,1);
        if (p.lengthSq() < 1.0) return p;
    }
}

Vec3 cross(const Vec3& b) const {
    return {
        y*b.z - z*b.y,
        z*b.x - x*b.z,
        x*b.y - y*b.x
    };
}
