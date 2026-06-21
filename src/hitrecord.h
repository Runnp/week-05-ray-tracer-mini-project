#pragma once
#include <memory>
#include "vec3.h"

struct Material;

struct HitRecord {
    Vec3                       point;
    Vec3                       normal;
    double                     t;
    std::shared_ptr<Material>  material;
};
