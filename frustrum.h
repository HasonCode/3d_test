#ifndef FRUSTRUM_H
#define FRUSTRUM_H

#include <glm/glm.hpp>
#include "AABB.h"
#include "camera.h"

struct Plane{
    glm::vec3 normal = {0.0f, 1.0f, 0.0f};
    glm::vec3 point = glm::vec3(0.0f);
    float offset = -glm::dot(normal,point);
};

struct Frustrum{
    Plane top_face;
    Plane bottom_face;

    Plane left_face;
    Plane right_face;

    Plane far_face;
    Plane near_face;
};

Frustrum extract_frustrum(const Camera &camera);
bool is_inside_frustrum(const Frustrum &frustrum, const AABB &aabb);

#endif