#pragma once
#include <memory>
#include "vec3.h"
#include "ray.h"
#include "material.h"

struct HitRecord {
    Vec3                       point;
    Vec3                       normal;
    double                     t;
    std::shared_ptr<Material>  material;
};

struct Sphere {
    Vec3                      center;
    double                    radius;
    std::shared_ptr<Material> material;  // NEW

    Sphere(Vec3 center, double radius, std::shared_ptr<Material> material)
        : center(center), radius(radius), material(material) {}

    bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const {
        Vec3 oc = ray.origin - center;
        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b*b - 4*a*c;

        if (discriminant < 0) return false;

        double sqrtD = std::sqrt(discriminant);
        double root  = (-b - sqrtD) / (2.0 * a);
        if (root < tMin || root > tMax) {
            root = (-b + sqrtD) / (2.0 * a);
            if (root < tMin || root > tMax) return false;
        }

        rec.t        = root;
        rec.point    = ray.at(root);
        rec.normal   = (rec.point - center).normalize();
        rec.material = material;
        return true;
    }
};
