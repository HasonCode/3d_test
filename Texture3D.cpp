#include <GL/glew.h>
#include <iostream>
#include "Texture3D.h"
#include <vector>
#include <string>
#include "stb_image.h"
using namespace std;

// Constructor implementations
Texture3D::Texture3D(vector<string> faces) : face_textures(faces) {
    max_filter = GL_LINEAR;
    min_filter = GL_LINEAR;
    wrap_s = GL_CLAMP_TO_EDGE;
    wrap_t = GL_CLAMP_TO_EDGE;
    wrap_r = GL_CLAMP_TO_EDGE;
}

Texture3D::Texture3D() {
    max_filter = GL_LINEAR;
    min_filter = GL_LINEAR;
    wrap_s = GL_CLAMP_TO_EDGE;
    wrap_t = GL_CLAMP_TO_EDGE;
    wrap_r = GL_CLAMP_TO_EDGE;
}

Texture3D::Texture3D(string right, string left, string top, string bottom, string back, string front) {
    face_textures.push_back(right);
    face_textures.push_back(left);
    face_textures.push_back(top);
    face_textures.push_back(bottom);
    face_textures.push_back(back);
    face_textures.push_back(front);
    max_filter = GL_LINEAR;
    min_filter = GL_LINEAR;
    wrap_s = GL_CLAMP_TO_EDGE;
    wrap_t = GL_CLAMP_TO_EDGE;
    wrap_r = GL_CLAMP_TO_EDGE;
}
GLuint Texture3D::generate(){
    GLuint textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int height, width, nrChannels;
    for (GLuint i = 0; i < face_textures.size(); i++){
        unsigned char* data = stbi_load(face_textures[i].c_str(),&width,&height,&nrChannels,0);
        if (data){
            if (nrChannels==3){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB, GL_UNSIGNED_BYTE,data);
            }
            else if (nrChannels==4){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,data);
            }
            stbi_image_free(data);
        }
        else{
            cerr<<"Cubemap texture failed to load at path "<<face_textures[i] <<endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, max_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    this->ID = textureID;
    return textureID;
}
GLuint Texture3D::generate(vector<string> faces){
    face_textures = faces;
    GLuint textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int height, width, nrChannels;
    for (GLuint i = 0; i < face_textures.size(); i++){
        unsigned char* data = stbi_load(face_textures[i].c_str(),&width,&height,&nrChannels,0);
        if (data){
            if (nrChannels==3){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB, GL_UNSIGNED_BYTE,data);
            }
            else if (nrChannels==4){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,data);
            }
            stbi_image_free(data);
        }
        else{
            cerr<<"Cubemap texture failed to load at path "<<face_textures[i] <<endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, max_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    this->ID = textureID;
    return textureID;
}

void Texture3D::bind(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
}