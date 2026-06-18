#include <iostream>
#include <fstream>
#include <vector>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"

// Test ray against every sphere, return closest hit
bool hitScene(const std::vector<Sphere>& scene, const Ray& ray,
              double tMin, double tMax, HitRecord& rec) {
    HitRecord temp;
    bool hitAnything  = false;
    double closestSoFar = tMax;

    for (const Sphere& sphere : scene) {
        if (sphere.hit(ray, tMin, closestSoFar, temp)) {
            hitAnything   = true;
            closestSoFar  = temp.t;   // tighten the window
            rec           = temp;
        }
    }
    return hitAnything;
}

Vec3 rayColor(const Ray& ray, const std::vector<Sphere>& scene) {
    HitRecord rec;

    if (hitScene(scene, ray, 0.0, 1e9, rec)) {
        // Visualize normal as color
        return (rec.normal + Vec3(1,1,1)) * 0.5;
    }

    // Sky gradient
    Vec3 unitDir = ray.direction.normalize();
    double blend = 0.5 * (unitDir.y + 1.0);
    return Vec3(1,1,1) * (1.0 - blend) + Vec3(0.5, 0.7, 1.0) * blend;
}

int main() {
    // Image
    const double aspectRatio = 16.0 / 9.0;
    const int    imageWidth  = 400;
    const int    imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // Camera
    double viewportHeight = 2.0;
    double viewportWidth  = aspectRatio * viewportHeight;
    double focalLength    = 1.0;

    Vec3 origin(0, 0, 0);
    Vec3 horizontal(viewportWidth, 0, 0);
    Vec3 vertical(0, viewportHeight, 0);
    Vec3 lowerLeftCorner = origin
        - horizontal / 2
        - vertical   / 2
        - Vec3(0, 0, focalLength);

    // Scene — add as many spheres as you want here
    std::vector<Sphere> scene;
    scene.push_back(Sphere(Vec3( 0.0,    0.0, -1.0),  0.5));   // center sphere
    scene.push_back(Sphere(Vec3(-1.0,    0.0, -1.0),  0.5));   // left sphere
    scene.push_back(Sphere(Vec3( 1.0,    0.0, -1.0),  0.5));   // right sphere
    scene.push_back(Sphere(Vec3( 0.0, -100.5, -1.0), 100.0));  // ground

    // Output
    std::ofstream file("renders/image.ppm");
    file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            double u = double(i) / (imageWidth  - 1);
            double v = double(j) / (imageHeight - 1);

            Ray ray(origin, lowerLeftCorner + horizontal*u + vertical*v - origin);
            Vec3 color = rayColor(ray, scene);

            int ir = static_cast<int>(255.999 * color.r());
            int ig = static_cast<int>(255.999 * color.g());
            int ib = static_cast<int>(255.999 * color.b());

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}