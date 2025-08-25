#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

// #include "shader_utils.h"
// #include "Shader.h"
// #include "Texture3D.h"

// #include "fling.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "resourcemanager.h"

map<std::string,Shader> ResourceManager::shaders;
map<std::string,Texture3D> ResourceManager::textures;

Shader ResourceManager::load_shader(const string vertex_shader_file, const string fragment_shader_file, string name){
    shaders.emplace(name,load_shader_file(vertex_shader_file,fragment_shader_file));
    return shaders[name];

}
ResourceManager::ResourceManager(){}

Texture3D ResourceManager::load_texture(const vector<string> file, string name){
    textures.emplace(name,load_texture_file(file));
    return textures[name];
}

Texture3D ResourceManager::load_texture(string right, string left, string top, string bottom, string back, string front, string name){
    textures.emplace(name,load_texture_file(right, left, top, bottom, back, front));
    return textures[name];
}

Shader ResourceManager::get_shader(const string name){
    auto smth = shaders.find(name);
    if (smth != shaders.end()){
        return shaders[name];
    }
    return shaders[name];
}
void ResourceManager::clear(){
    for (auto iter: shaders){
        glDeleteProgram(iter.second.ID);
    }
    for(auto iter: textures){
        glDeleteTextures(1,&iter.second.ID);
    }
}
Texture3D ResourceManager::get_texture(const string name){
    return textures[name];
}
Shader ResourceManager::load_shader_file(const string vertex_shader_file, const string fragment_shader_file){
    return Shader(vertex_shader_file.c_str(),fragment_shader_file.c_str());
}

Texture3D ResourceManager::load_texture_file(string right, string left, string top, string bottom, string back, string front){
    Texture3D texture(right, left, top, bottom, back, front);
    texture.generate();
    return texture;
}

Texture3D ResourceManager::load_texture_file(vector<string> sides){
    Texture3D texture(sides);
    texture.generate();
    return texture;
}