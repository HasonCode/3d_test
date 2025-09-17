#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
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

#ifndef CUBE_RENDERER
#define CUBE_RENDERER



class CubeRenderer{
    public:
        Camera camera;
        GLint coord3d;
        GLint normal;
        CubeRenderer();
        CubeRenderer(Shader &shader, Camera camera);
        ~CubeRenderer();
        void drawCube(Texture3D &texture, glm::vec3 position,
            glm::vec3 size, glm::vec3 rotate, glm::vec3 color, glm::vec3 light_pos = glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f));  
    private:
        Shader shader;
        GLuint quadVAO;
        GLuint VBO;
        GLuint Normal_VBO;
        GLuint IBO;
        float* vertecies;
        float* normals;
        unsigned int* indexes;
        void initRenderData();  
};

#endif