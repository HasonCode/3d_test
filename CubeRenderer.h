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
        GLint coord3d;
        CubeRenderer();
        CubeRenderer(Shader &shader, Camera &camera);
        ~CubeRenderer();
        void drawCube(Texture3D &texture, glm::vec3 position,
            glm::vec3 size, glm::vec3 rotate, glm::vec3 color);  
    private:
        Shader shader;
        GLuint quadVAO;
        GLuint VBO;
        GLuint IBO;
        float* vertecies;
        unsigned int* indexes;
        Camera camera;
        void initRenderData();  
};

#endif