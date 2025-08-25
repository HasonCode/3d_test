#ifndef TEXTURE_3D
#define TEXTURE_3D
#include <vector>
#include <string>
#include <GL/glew.h>
class Texture3D{
    public:
        unsigned int ID;
        unsigned int width, height, depth;
        unsigned int internal_format;
        GLint max_filter;
        GLint min_filter;
        GLint wrap_s;
        GLint wrap_t;
        GLint wrap_r;
        std::vector<std::string> face_textures;
        Texture3D();
        Texture3D(std::vector<std::string> faces);
        Texture3D(std::string right, std::string left, std::string top, std::string bottom, std::string back, std::string front);
        void bind();
        GLuint generate();
        GLuint generate(std::vector<std::string>faces);
};

#endif