#include <GL/glew.h>
#include <iostream>
#include "Texture2D.h"

Texture2D::Texture2D(){
    this->width = 0;
    this->height = 0;
    this-> Internal_Format = GL_RGB;
    this-> Image_Format = GL_RGB;
    this-> Wrap_S = GL_CLAMP_TO_EDGE;
    this-> Wrap_T = GL_CLAMP_TO_EDGE;
    this->Filter_Min = GL_NEAREST;
    this->Filter_Max = GL_NEAREST;
    glGenTextures(1,&this->ID);
    
}
void Texture2D::Generate(unsigned int t_width, unsigned int t_height, unsigned char* data){
    this->width = t_width;
    this->height = t_height;
    glBindTexture(GL_TEXTURE_2D,this->ID);
    glTexImage2D(GL_TEXTURE_2D,0,this->Internal_Format,this->width,this->height,0,this->Image_Format,GL_UNSIGNED_BYTE,data);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "[OpenGL Error after glTexImage2D] " << err << std::endl;
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,this->Filter_Max);
    glBindTexture(GL_TEXTURE_2D,0);
}
void Texture2D::bind(){
    glBindTexture(GL_TEXTURE_2D,this->ID);
}