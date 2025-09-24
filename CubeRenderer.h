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

struct PointLight{
    glm::vec3 position;

    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambience;

    float constant;
    float linear;
    float quadratic;
};


struct SpotLight{
    glm::vec3 position;
    glm::vec3 direction;
    float cut_off;
    float outer_cut_off;

    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambience;

    float constant;
    float linear;
    float quadratic;
};


class CubeRenderer{
    public:
        Camera camera;
        GLint coord3d;
        GLint normal;
        CubeRenderer();
        CubeRenderer(Shader &shader, Camera camera);
        ~CubeRenderer();
        void drawCube(Texture3D &texture, glm::vec3 position,
            glm::vec3 size, glm::vec3 rotate, glm::vec3 color, vector<PointLight> point_lights, vector<SpotLight> spot_lights, float shininess);  
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