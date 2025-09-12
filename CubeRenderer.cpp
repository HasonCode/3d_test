#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
#include "CubeRenderer.h"
#include "camera.h"
#include "Shader.h"
#include "Texture3D.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Constructor implementations
CubeRenderer::CubeRenderer(){
    shader = Shader();
}

CubeRenderer::CubeRenderer(Shader &shader, Camera camera): shader(shader), camera(camera){
    this->initRenderData();
    this->shader.use();
    this->shader.set_sampler("cubemap",0);
    // Get attribute location from shader
    this->coord3d = glGetAttribLocation(shader.ID, "a_pos");
    this->normal = glGetAttribLocation(shader.ID, "a_normal");
}

CubeRenderer::~CubeRenderer(){
    glDeleteVertexArrays(1,&this->quadVAO);
}
void CubeRenderer::drawCube(Texture3D &texture, glm::vec3 position,
    glm::vec3 size, glm::vec3 rotate, glm::vec3 color, glm::vec3 light_pos, glm::vec3 light_color){
    //Begin shader usage
    this->shader.use();
    this->shader.set_sampler("cubemap", 0);
    const int indices_in_cube = 36;  // 12 triangles * 3 vertices each

    //Matrix Processing
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size));
    glm::mat4 move_to_anchor = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotatex = glm::rotate(glm::mat4(1.0f), rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotatey = glm::rotate(glm::mat4(1.0f), rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotatez = glm::rotate(glm::mat4(1.0f), rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation = rotatez * rotatey * rotatex;
    glm::mat4 model = move_to_anchor * rotation * scale;  // Removed problematic offset

    //Camera Creation:
    glm::mat4 view = glm::lookAt(this->camera.position, this->camera.target, this->camera.up);
    glm::mat4 projection = glm::perspective(this->camera.fov, this->camera.aspect_ratio, this->camera.near, this->camera.far);

    this->shader.set_vector3f("camera_pos",camera.position, true);
    this->shader.set_matrix4("projection",projection,true);
    this->shader.set_matrix4("model",model,true);
    this->shader.set_matrix4("view",view,true);
    this->shader.set_vector3f("color",color,true);
    this->shader.set_vector3f("light.position", light_pos, true);
    this->shader.set_vector3f("material.ambience", glm::vec3(0.1f, 0.1f, 0.1f), true);
    this->shader.set_vector3f("material.diffuse", glm::vec3(0.4f, 0.4f, 0.4f), true);
    this->shader.set_vector3f("material.specular", glm::vec3(0.6f, 0.6f, 0.6f), true);
    this->shader.set_vector3f("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f), true);
    this->shader.set_vector3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f), true);
    this->shader.set_vector3f("light.ambience", glm::vec3(1.0f, 1.0f, 1.0f), true);

    this->shader.set_float("material.shininess", 32.0f);
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glEnableVertexAttribArray(this->coord3d);
    glVertexAttribPointer(
        this->coord3d,
        3,
        GL_FLOAT,
        GL_FALSE,
        3*sizeof(float),  // Fixed stride for position-only data
        0
      );
    glBindBuffer(GL_ARRAY_BUFFER, this->Normal_VBO);
    glEnableVertexAttribArray(this->normal);
    glVertexAttribPointer(
        this->normal,
        3,
        GL_FLOAT,
        GL_FALSE,
        3*sizeof(float),  // Fixed stride for position-only data
        0
      );
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
      glDrawElements(GL_TRIANGLES, indices_in_cube, GL_UNSIGNED_INT, 0);
}

void CubeRenderer::initRenderData(){
    // Create and bind VAO first
    glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO);

    float cubeVertices[] = {
                // Front face (Z = 1)
                -1.0f, -1.0f,  1.0f,  // 0
                 1.0f, -1.0f,  1.0f,  // 1
                 1.0f,  1.0f,  1.0f,  // 2
                -1.0f,  1.0f,  1.0f,  // 3

                // Back face (Z = -1)
                -1.0f, -1.0f, -1.0f,  // 4
                 1.0f, -1.0f, -1.0f,  // 5
                 1.0f,  1.0f, -1.0f,  // 6
                -1.0f,  1.0f, -1.0f,  // 7

                // Left face (X = -1)
                -1.0f, -1.0f, -1.0f,  // 8
                -1.0f, -1.0f,  1.0f,  // 9
                -1.0f,  1.0f,  1.0f,  // 10
                -1.0f,  1.0f, -1.0f,  // 11

                // Right face (X = 1)
                 1.0f, -1.0f, -1.0f,  // 12
                 1.0f, -1.0f,  1.0f,  // 13
                 1.0f,  1.0f,  1.0f,  // 14
                 1.0f,  1.0f, -1.0f,  // 15

                // Top face (Y = 1)
                -1.0f,  1.0f, -1.0f,  // 16
                -1.0f,  1.0f,  1.0f,  // 17
                 1.0f,  1.0f,  1.0f,  // 18
                 1.0f,  1.0f, -1.0f,  // 19

                // Bottom face (Y = -1)
                -1.0f, -1.0f, -1.0f,  // 20
                -1.0f, -1.0f,  1.0f,  // 21
                 1.0f, -1.0f,  1.0f,  // 22
                 1.0f, -1.0f, -1.0f   // 23
            };
    float normals[] = {

        0.0f, 0.0f, 1.0f,  // 0
        0.0f, 0.0f, 1.0f,  // 1
        0.0f, 0.0f, 1.0f,  // 2
        0.0f, 0.0f, 1.0f,  // 3

        0.0f, 0.0f, -1.0f,  // 4
        0.0f, 0.0f, -1.0f,  // 5   
        0.0f, 0.0f, -1.0f,  // 6
        0.0f, 0.0f, -1.0f,  // 7

        -1.0f, 0.0f, 0.0f,  // 8
        -1.0f, 0.0f, 0.0f,  // 9    
        -1.0f, 0.0f, 0.0f,  // 10
        -1.0f, 0.0f, 0.0f,  // 11

        1.0f, 0.0f, 0.0f,  // 12
        1.0f, 0.0f, 0.0f,  // 13
        1.0f, 0.0f, 0.0f,  // 14
        1.0f, 0.0f, 0.0f,  // 15

        0.0f, 1.0f, 0.0f,  // 16
        0.0f, 1.0f, 0.0f,  // 17
        0.0f, 1.0f, 0.0f,  // 18
        0.0f, 1.0f, 0.0f,  // 19

        0.0f, -1.0f, 0.0f,  // 20
        0.0f, -1.0f, 0.0f,  // 21
        0.0f, -1.0f, 0.0f,  // 22
        0.0f, -1.0f, 0.0f   // 23

    };

    unsigned int cubeIndices[] = {
        // Front face
        0, 1, 2,   2, 3, 0,

        // Back face
        4, 6, 5,   6, 4, 7,

        // Left face
        8, 9, 10,  10, 11, 8,

        // Right face
        12, 14, 13, 14, 12, 15,

        // Top face
        16, 17, 18, 18, 19, 16,

        // Bottom face
        20, 22, 21, 22, 20, 23
    };

    //Create Cube
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),
        cubeVertices, GL_STATIC_DRAW);

    //Create Normals
    glGenBuffers(1, &Normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    
    //Create Linkings for Cube
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices),
        cubeIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(this->coord3d);
    glVertexAttribPointer(
      this->coord3d,
      3,
      GL_FLOAT,
      GL_FALSE,
      3*sizeof(float),  // Fixed stride for position-only data
      0
    );
    glBindBuffer(GL_ARRAY_BUFFER, Normal_VBO);
    glEnableVertexAttribArray(this->normal);
    glVertexAttribPointer(
      this->normal,
      3,
      GL_FLOAT,
      GL_FALSE,
      3*sizeof(float),  // Fixed stride for position-only data
      0
    );
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

