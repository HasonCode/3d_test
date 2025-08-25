#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

// #include "shader_utils.h"
#include "Shader.h"
#include "Texture3D.h"

// #include "fling.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include "stb_image.h"
#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER
class ResourceManager{
    public:
        static map<std::string, Shader> shaders;
        static map<std::string, Texture3D> textures;

        static Shader load_shader(const string vertex_shader_file, const string fragment_shader_file, string name);

        static Texture3D load_texture(const vector<string> file, string name);

        static Texture3D load_texture(string right, string left, string top, string bottom, string back, string front, string name);

        static Shader get_shader(string name); 
        
        static Texture3D get_texture(string name);

        ResourceManager();

        static void clear();
    private:
        

        static Shader load_shader_file(const string vertex_shader_file, const string fragment_shader_file);

        static Texture3D load_texture_file(string right, string left, string top, string bottom, string back, string front);

        static Texture3D load_texture_file(vector<string> file);

};


#endif