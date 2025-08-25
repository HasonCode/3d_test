#ifndef TEXTURE_2D
#define TEXTURE_2D
#include <GL/glew.h>

class Texture2D{
    public:
        unsigned int ID;

        unsigned int width, height;
        
        unsigned int Internal_Format;
        unsigned int Image_Format;

        GLint Wrap_S;
        GLint Wrap_T;
        GLint Filter_Min;
        GLint Filter_Max;
        
        Texture2D();

        void Generate(unsigned int t_width, unsigned int t_height, unsigned char* data);
        
        void bind();
};

#endif