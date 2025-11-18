#include "AABB.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "frustrum.h"
#include "camera.h"


Frustrum extract_frustrum(const Camera &camera){
    Frustrum frustrum;
    
    // Build view and projection matrices (same as in CubeRenderer)
    glm::mat4 view = glm::lookAt(camera.position, camera.target, camera.up);
    glm::mat4 projection = glm::perspective(camera.fov, camera.aspect_ratio, camera.near, camera.far);
    glm::mat4 vp = projection * view;
    
    // Extract planes from combined matrix
    // Planes are in the form: ax + by + cz + d = 0, stored as (a,b,c,d)
    // For column-major GLM, extract from rows of vp
    
    auto extractPlane = [](const glm::mat4& m, int row) -> Plane {
        Plane p;
        // Extract plane equation coefficients
        glm::vec4 plane;
        if (row == 0) {  // Left
            plane = glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]);
        } else if (row == 1) {  // Right
            plane = glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]);
        } else if (row == 2) {  // Bottom
            plane = glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]);
        } else if (row == 3) {  // Top
            plane = glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]);
        } else if (row == 4) {  // Near
            plane = glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]);
        } else {  // Far (row == 5)
            plane = glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]);
        }
        
        // Normalize
        float len = glm::length(glm::vec3(plane));
        if (len > 0.0f) {
            plane /= len;
        }
        
        p.normal = glm::vec3(plane);
        p.offset = plane.w;
        // Calculate a point on the plane for debugging (not strictly needed)
        p.point = -p.normal * p.offset;
        
        return p;
    };
    
    frustrum.left_face = extractPlane(vp, 0);
    frustrum.right_face = extractPlane(vp, 1);
    frustrum.bottom_face = extractPlane(vp, 2);
    frustrum.top_face = extractPlane(vp, 3);
    frustrum.near_face = extractPlane(vp, 4);
    frustrum.far_face = extractPlane(vp, 5);
    
    return frustrum;
}

bool is_inside_frustrum(const Frustrum &frustrum, const AABB &aabb){
    //near_face:
    float x = frustrum.near_face.normal.x>0 ? aabb.max.x : aabb.min.x;
    float y = frustrum.near_face.normal.y>0 ? aabb.max.y : aabb.min.y;
    float z = frustrum.near_face.normal.z>0 ? aabb.max.z : aabb.min.z;

    float dist = glm::dot(frustrum.near_face.normal, glm::vec3(x,y,z)) + frustrum.near_face.offset;
    if(dist<0){
        return false;
    }

    //far_face:
    x = frustrum.far_face.normal.x>0 ? aabb.max.x : aabb.min.x;
    y = frustrum.far_face.normal.y>0 ? aabb.max.y : aabb.min.y;
    z = frustrum.far_face.normal.z>0 ? aabb.max.z : aabb.min.z;

    dist = glm::dot(frustrum.far_face.normal, glm::vec3(x,y,z)) + frustrum.far_face.offset;
    if(dist<0){
        return false;
    }

    //right_face:
    x = frustrum.right_face.normal.x>0 ? aabb.max.x : aabb.min.x;
    y = frustrum.right_face.normal.y>0 ? aabb.max.y : aabb.min.y;
    z = frustrum.right_face.normal.z>0 ? aabb.max.z : aabb.min.z;

    dist = glm::dot(frustrum.right_face.normal, glm::vec3(x,y,z)) + frustrum.right_face.offset;
    if(dist<0){
        return false;
    }

    //left_face:
    x = frustrum.left_face.normal.x>0 ? aabb.max.x : aabb.min.x;
    y = frustrum.left_face.normal.y>0 ? aabb.max.y : aabb.min.y;
    z = frustrum.left_face.normal.z>0 ? aabb.max.z : aabb.min.z;

    dist = glm::dot(frustrum.left_face.normal, glm::vec3(x,y,z)) + frustrum.left_face.offset;
    if(dist<0){
        return false;
    }

    //top_face:
    x = frustrum.top_face.normal.x>0 ? aabb.max.x : aabb.min.x;
    y = frustrum.top_face.normal.y>0 ? aabb.max.y : aabb.min.y;
    z = frustrum.top_face.normal.z>0 ? aabb.max.z : aabb.min.z;

    dist = glm::dot(frustrum.top_face.normal, glm::vec3(x,y,z)) + frustrum.top_face.offset;
    if(dist<0){
        return false;
    }

    //bottom_face:
    x = frustrum.bottom_face.normal.x>0 ? aabb.max.x : aabb.min.x;
    y = frustrum.bottom_face.normal.y>0 ? aabb.max.y : aabb.min.y;
    z = frustrum.bottom_face.normal.z>0 ? aabb.max.z : aabb.min.z;

    dist = glm::dot(frustrum.bottom_face.normal, glm::vec3(x,y,z))+frustrum.bottom_face.offset;
    if(dist<0){
        return false;
    }
    return true;
}