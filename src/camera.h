#pragma once
#include <cmath>
#include "vec3.h"
#include "ray.h"

const double PI = 3.14159265358979323846;

inline double degreesToRadians(double degrees) {
    return degrees * PI / 180.0;
}

struct Camera {
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera(
        Vec3   lookFrom,
        Vec3   lookAt,
        Vec3   vup,
        double vfov,
        double aspectRatio
    ) {
        double theta          = degreesToRadians(vfov);
        double viewportHeight = 2.0 * std::tan(theta / 2.0);
        double viewportWidth  = aspectRatio * viewportHeight;

        Vec3 forward = (lookFrom - lookAt).normalize();
        Vec3 right   = vup.cross(forward).normalize();
        Vec3 up      = forward.cross(right);

        origin          = lookFrom;
        horizontal      = right * viewportWidth;
        vertical        = up    * viewportHeight;
        lowerLeftCorner = origin
            - horizontal / 2
            - vertical   / 2
            - forward;
    }

    Ray getRay(double u, double v) const {
        Vec3 direction = lowerLeftCorner
            + horizontal * u
            + vertical   * v
            - origin;
        return Ray(origin, direction);
    }
};
