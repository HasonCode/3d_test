#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

Camera::Camera(glm::vec3 init_pos, glm::vec3 init_target){
    this->up = glm::vec3(0.0,1.0,0.0);
    // this->down = glm::vec3(0.0,-1.0,0.0);
    // this->front = glm::vec3(0.0,0.0,1.0);
    // this->back = glm::vec3(0.0,0.0,-1.0);
    // this->left = glm::vec3(1.0,0.0,0.0);
    // this->right = glm::vec3(-1.0,0.0,0.0);

    this->position = init_pos;
    this->target = init_target;
    this->fov = glm::radians(45.0f);
    this->far = 1000;
    this->near = 0.1;
}

Camera::Camera(){
    this->up = glm::vec3(0.0,1.0,0.0);
    // this->down = glm::vec3(0.0,-1.0,0.0);
    // this->front = glm::vec3(0.0,0.0,1.0);
    // this->back = glm::vec3(0.0,0.0,-1.0);
    // this->left = glm::vec3(1.0,0.0,0.0);
    // this->right = glm::vec3(-1.0,0.0,0.0);
    this->position = glm::vec3(0.0f);
    this->target = glm::vec3(0.0f);
    this->fov = glm::radians(45.0f);
    this->far = 1000;
    this->near = 0.1;
}

void Camera::update_screen(float new_height, float new_width){
    this->screen_height = new_height;
    this->screen_width = new_width;
    update_camera();
}

float Camera::aspect_axis(){
    float outputzoom = 1.0f;
    float aspectorigin = 16.0f / 9.0f;
    int aspectconstraint = 1;
    switch(aspectconstraint){
        case 1:
            if ((screen_width/screen_height)<aspectorigin){
                outputzoom *= (((float)this->screen_width/this->screen_height)/aspectorigin);
            }
            else{
                outputzoom*= ((float)aspectorigin/aspectorigin);
            }
            break;
        case 2: 
            outputzoom *= (((float)this->screen_width/this->screen_height)/aspectorigin);
            break;
        default:
            outputzoom *= ((float)aspectorigin/aspectorigin);
    }
    return outputzoom;
}

float Camera::recalculate_fov(){
    return 2.0f * glm::atan(glm::tan(glm::radians(45.0f/2.0f))/aspect_axis());
}

void Camera::move_camera(float x, float y, float z){
    this->position[0] = x;
    this->position[1] = y;
    this->position[2] = z;
    update_camera();
}

void Camera::change_target(float x, float y, float z){
    this->target[0] = x;
    this->target[1] = y;
    this->target[2] = z;
    update_camera();
}

void Camera::update_camera(){
    this->fov = glm::radians(45.0f);
    this->aspect_ratio = 1.0f*screen_width/screen_height;
}