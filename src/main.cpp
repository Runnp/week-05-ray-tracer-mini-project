#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "material.h"
#include "sphere.h"

bool hitScene(const std::vector<Sphere>& scene, const Ray& ray,
              double tMin, double tMax, HitRecord& rec) {
    HitRecord temp;
    bool   hitAnything  = false;
    double closestSoFar = tMax;

    for (const Sphere& sphere : scene) {
        if (sphere.hit(ray, tMin, closestSoFar, temp)) {
            hitAnything   = true;
            closestSoFar  = temp.t;
            rec           = temp;
        }
    }
    return hitAnything;
}

Vec3 rayColor(const Ray& ray, const std::vector<Sphere>& scene, int depth) {
    if (depth <= 0) return Vec3(0, 0, 0);

    HitRecord rec;
    if (hitScene(scene, ray, 0.001, 1e9, rec)) {
        Ray  scattered(ray.origin, ray.direction);
        Vec3 attenuation;
        if (rec.material->scatter(ray, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, scene, depth - 1);
        }
        return Vec3(0, 0, 0);
    }

    Vec3 unitDir = ray.direction.normalize();
    double blend = 0.5 * (unitDir.y + 1.0);
    return Vec3(1,1,1) * (1.0 - blend) + Vec3(0.5, 0.7, 1.0) * blend;
}

int main() {
    const double aspectRatio = 16.0 / 9.0;
    const int    imageWidth  = 400;
    const int    imageHeight = static_cast<int>(imageWidth / aspectRatio);

    double viewportHeight    = 2.0;
    double viewportWidth     = aspectRatio * viewportHeight;
    double focalLength       = 1.0;

    Vec3 origin(0, 0, 0);
    Vec3 horizontal(viewportWidth, 0, 0);
    Vec3 vertical(0, viewportHeight, 0);
    Vec3 lowerLeftCorner = origin
        - horizontal / 2
        - vertical   / 2
        - Vec3(0, 0, focalLength);

    auto groundMaterial = std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto centerMaterial = std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
    auto leftMaterial   = std::make_shared<Dielectric>(1.5);
    auto rightMaterial  = std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.0);

    std::vector<Sphere> scene;
    scene.push_back(Sphere(Vec3( 0.0,    0.0, -1.0),  0.5,  centerMaterial));
    scene.push_back(Sphere(Vec3(-1.0,    0.0, -1.0),  0.5,  leftMaterial));
    scene.push_back(Sphere(Vec3(-1.0,    0.0, -1.0), -0.4,  leftMaterial));  // hollow
    scene.push_back(Sphere(Vec3( 1.0,    0.0, -1.0),  0.5,  rightMaterial));
    scene.push_back(Sphere(Vec3( 0.0, -100.5, -1.0), 100.0, groundMaterial));

    const int samplesPerPixel = 50;
    const int maxDepth        = 50;

    std::ofstream file("renders/image.ppm");
    file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            Vec3 color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; s++) {
                double u = (i + randomDouble()) / (imageWidth  - 1);
                double v = (j + randomDouble()) / (imageHeight - 1);
                Ray ray(origin, lowerLeftCorner + horizontal*u + vertical*v - origin);
                color = color + rayColor(ray, scene, maxDepth);
            }

            color = color / samplesPerPixel;

            double r = std::sqrt(color.r());
            double g = std::sqrt(color.g());
            double b = std::sqrt(color.b());

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}