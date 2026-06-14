#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"

// Returns t if the ray hits the sphere, -1 if it misses
// Math: solve |P(t) - center|^2 = radius^2
//       expands to quadratic: at^2 + bt + c = 0
double hitSphere(const Vec3& center, double radius, const Ray& ray) {
    Vec3 oc = ray.origin - center;

    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;

    double discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return -1.0;           // ray misses
    } else {
        return (-b - std::sqrt(discriminant)) / (2.0 * a);  // nearest hit
    }
}

// What color does this ray see?
Vec3 rayColor(const Ray& ray) {
    double t = hitSphere(Vec3(0, 0, -1), 0.5, ray);

    if (t > 0.0) {
        return Vec3(1, 0, 0);  // red sphere
    }

    // Sky gradient: white at bottom, light blue at top
    Vec3 unitDir = ray.direction.normalize();
    double blend = 0.5 * (unitDir.y + 1.0);  // remap y [-1,1] -> [0,1]
    return Vec3(1,1,1) * (1.0 - blend) + Vec3(0.5, 0.7, 1.0) * blend;
}

int main() {
    // Image
    const double aspectRatio = 16.0 / 9.0;
    const int    imageWidth  = 400;
    const int    imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // Camera / viewport
    double viewportHeight = 2.0;
    double viewportWidth  = aspectRatio * viewportHeight;
    double focalLength    = 1.0;

    Vec3 origin     (0, 0, 0);
    Vec3 horizontal (viewportWidth, 0, 0);
    Vec3 vertical   (0, viewportHeight, 0);

    // Bottom-left corner of the viewport in world space
    Vec3 lowerLeftCorner = origin
        - horizontal / 2
        - vertical   / 2
        - Vec3(0, 0, focalLength);

    // Output file
    std::ofstream file("renders/image.ppm");

    // PPM header
    file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    // Render — row by row, top to bottom
    for (int j = imageHeight - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;

        for (int i = 0; i < imageWidth; i++) {
            // UV: which fraction across/up the viewport is this pixel?
            double u = double(i) / (imageWidth  - 1);
            double v = double(j) / (imageHeight - 1);

            Ray ray(origin, lowerLeftCorner + horizontal*u + vertical*v - origin);
            Vec3 color = rayColor(ray);

            // Convert [0,1] floats to [0,255] ints
            int ir = static_cast<int>(255.999 * color.r());
            int ig = static_cast<int>(255.999 * color.g());
            int ib = static_cast<int>(255.999 * color.b());

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}