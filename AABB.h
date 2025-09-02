#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;
#ifndef AABBMANAGER
#define AABBMANAGER
struct AABB{
    glm::vec3 min;
    glm::vec3 max;
};
class AABBManager{
    public:
        std::vector<AABB> aabbs;
        AABBManager();
        void add_aabb(AABB to_add);
        bool check_collision(AABB to_check);
        static bool check_intersect(AABB aabb1, AABB aabb2);
};

#endif