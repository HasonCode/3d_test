#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "shader_utils.h"
class Shader{
    public:
        GLint ID;
        Shader(const char* vertex_path, const char* fragment_path);
        Shader();
        void use();
        void set_bool(const string &name, bool value);
        void set_int(const string &name, int value);
        void set_float(const string &name, float value);
        void set_sampler(const string &name, int unit);
        void set_vector2f(const string &name, float x, float y, bool use_shader);
        void set_vector2f(const string &name, glm::vec2 vals, bool use_shader);
        void set_vector3f(const string &name, float x, float y,float z, bool use_shader);
        void set_vector3f(const string &name, glm::vec3 vals, bool use_shader);
        void set_vector4f(const string &name, float x, float y, float z, float w, bool use_shader);
        void set_vector4f(const string &name, glm::vec4 vals, bool use_shader); 
        void set_matrix4(const string &name, glm::mat4 matrix, bool use_shader);
        GLint attach_attribute(const string &name);

};




#endif