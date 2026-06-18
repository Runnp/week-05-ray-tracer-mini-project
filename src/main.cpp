#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"

double hitSphere(const Vec3& center, double radius, const Ray& ray) {
    Vec3 oc = ray.origin - center;
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0) return -1.0;
    return (-b - std::sqrt(discriminant)) / (2.0 * a);
}

Vec3 rayColor(const Ray& ray) {
    double t = hitSphere(Vec3(0, 0, -1), 0.5, ray);

    if (t > 0.0) {
        Vec3 hitPoint = ray.at(t);
        Vec3 normal   = (hitPoint - Vec3(0, 0, -1)).normalize();

        return (normal + Vec3(1,1,1)) * 0.5;
    }

    Vec3 unitDir = ray.direction.normalize();
    double blend = 0.5 * (unitDir.y + 1.0);
    return Vec3(1,1,1) * (1.0 - blend) + Vec3(0.5, 0.7, 1.0) * blend;
}

int main() {
    
    const double aspectRatio = 16.0 / 9.0;
    const int    imageWidth  = 400;
    const int    imageHeight = static_cast<int>(imageWidth / aspectRatio);

    double viewportHeight = 2.0;
    double viewportWidth  = aspectRatio * viewportHeight;
    double focalLength    = 1.0;

    Vec3 origin     (0, 0, 0);
    Vec3 horizontal (viewportWidth, 0, 0);
    Vec3 vertical   (0, viewportHeight, 0);

    Vec3 lowerLeftCorner = origin
        - horizontal / 2
        - vertical   / 2
        - Vec3(0, 0, focalLength);

    std::ofstream file("renders/image.ppm");

    file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;

        for (int i = 0; i < imageWidth; i++) {
            
            double u = double(i) / (imageWidth  - 1);
            double v = double(j) / (imageHeight - 1);

            Ray ray(origin, lowerLeftCorner + horizontal*u + vertical*v - origin);
            Vec3 color = rayColor(ray);

            int ir = static_cast<int>(255.999 * color.r());
            int ig = static_cast<int>(255.999 * color.g());
            int ib = static_cast<int>(255.999 * color.b());

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
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