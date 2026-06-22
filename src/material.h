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

struct Metal : public Material {
    Vec3   albedo;
    double fuzz;

    Metal(const Vec3& albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec,
                 Vec3& attenuation, Ray& scattered) const override {
        Vec3 unitIn    = rayIn.direction.normalize();
        Vec3 reflected = unitIn - rec.normal * 2.0 * unitIn.dot(rec.normal);

        scattered   = Ray(rec.point, reflected + randomInUnitSphere() * fuzz);
        attenuation = albedo;

        return scattered.direction.dot(rec.normal) > 0;
    }
};
