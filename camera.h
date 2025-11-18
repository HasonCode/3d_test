#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef CAMERA
#define CAMERA
class Camera{
    public:
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 down;
        glm::vec3 left;
        glm::vec3 front;
        glm::vec3 back;

        float fov;
        float aspect_ratio;
        float near;
        float far;
        float screen_height;
        float screen_width;
        Camera(glm::vec3 init_pos, glm::vec3 init_target);
        Camera();
        void update_screen(float new_height, float new_width);
        float aspect_axis();
        float recalculate_fov();
        void move_camera(float x, float y, float z);
        void change_target(float x, float y, float z);
        void update_camera();
};



#endif