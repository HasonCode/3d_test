#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

#include <SDL2/SDL_mixer.h>
#include "shader_utils.h"
#include "Shader.h"
#include "Texture3D.h"
#include "CubeRenderer.h"
#include "resourcemanager.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SpriteRenderer.h"
//Program
GLint program;
//Attributes
GLint attribute_coord2d, attribute_v_color, attribute_texture_coord;
//Uniforms
GLint uniform_fade;

//Buffers 
GLuint vbo_cords, vbo_colors;
GLuint ibo_grids;
GLuint vbo_fling_cords, vbo_fling_colors;
GLuint ibo_flings;
GLuint vbo_texture_coords;
//Screen Size 
int screen_width= 800, screen_height=1000;
//Window
SDL_Window* window;

//Texture



void render (SDL_Window* window, Shader s, CubeRenderer renderer){
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    int x = 0, y = 0, z = 0;
    glm::vec3 scale = glm::vec3(10, 10, 10);
    glm::vec3 rotation = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
    Texture3D texture = ResourceManager::get_texture("testing_cube");
    renderer.drawCube(texture, glm::vec3(x, y, z), scale, rotation, color);
    SDL_GL_SwapWindow(window);
}

void logic(Shader s){
    //attach uniform
    s.use();
    glActiveTexture(GL_TEXTURE0);
}

void on_resize(int width, int height){
    screen_width = width;
    screen_height = height;
    glViewport(0,0,screen_width,screen_height);
}

CubeRenderer setup(Shader& shader, Camera& camera){
    CubeRenderer renderer(shader, camera);
    return renderer;
}

void main_loop(SDL_Window* window, Shader s, CubeRenderer renderer){
    while(true){
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            if(ev.type==SDL_QUIT){
                return;
            }
            if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                on_resize(ev.window.data1,ev.window.data2);
            }
        }
        logic(s);
        render(window,s, renderer);
    }
}

void free_resources(){
    glDeleteProgram(program);
    glDeleteBuffers(1,&vbo_cords);
    glDeleteBuffers(1,&vbo_colors);
    glDeleteBuffers(1,&ibo_grids);
    glDeleteBuffers(1, &vbo_fling_cords);
    glDeleteBuffers(1, &vbo_fling_colors);
    glDeleteBuffers(1, &ibo_flings);

}

void test_render();

int main(int argc, char* argv[]){
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
    window = SDL_CreateWindow("It's a Fling Party Out Here",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screen_width,screen_height,
        SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL
    );
    SDL_GL_CreateContext(window);
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK){
        cerr << "Error: glewInit" << glewGetErrorString(glew_status)<<endl;
        return EXIT_FAILURE;
    }
    Camera camera(glm::vec3(20, 20, 20), glm::vec3(5, 5, 5));
    camera.update_screen(screen_height, screen_width);
    Shader shader("shaders/cubemap.vert", "shaders/cubemap.frag");
    CubeRenderer renderer(shader, camera);
    ResourceManager::load_texture("sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "testing_cube");
    ResourceManager::load_shader("shaders/cubemap.vert", "shaders/cubemap.frag", "cubemap");
    glViewport(0,0,screen_width,screen_height);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,1);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    main_loop(window, shader, renderer);
    return EXIT_SUCCESS;
}