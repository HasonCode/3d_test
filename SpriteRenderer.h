#ifndef SPRITE_RENDERER
#define SPRITE_RENDERER
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// #include "shader_utils.h"
#include "Shader.h"
// #include "fling.h"
#include "Texture2D.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include "stb_image.h"
class SpriteRenderer{
    public:
        GLint coord2d;
        GLint textt;
        SpriteRenderer();
        SpriteRenderer(Shader& shader);
        ~SpriteRenderer();
        void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0,10.0), float rotate = 0.0, glm::vec3 color = glm::vec3(1.0f));
    private:
        Shader shader;
        GLuint quadVAO;
        GLuint VBO;
        void initRenderData();
    };

#endif