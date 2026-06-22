#pragma once
#include "vec3.h"
#include "ray.h"

struct HitRecord;

struct Material {
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray&       rayIn,
        const HitRecord& rec,
        Vec3&            attenuation,
        Ray&             scattered
    ) const = 0;
};

struct Lambertian : public Material {
    Vec3 albedo;

    Lambertian(const Vec3& albedo) : albedo(albedo) {}

    bool scatter(const Ray&, const HitRecord& rec,
                 Vec3& attenuation, Ray& scattered) const override {
        Vec3 scatterDir = rec.normal + randomInUnitSphere().normalize();
        scattered    = Ray(rec.point, scatterDir);
        attenuation  = albedo;
        return true;
    }
};