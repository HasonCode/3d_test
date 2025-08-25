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
// #include "shader_utils.h"
#include "Shader.h"
        Shader::Shader(){
            this->ID = 0;
        }
        Shader::Shader(const char* vertex_path, const char* fragment_path){
            GLuint vertex = create_shader(vertex_path, GL_VERTEX_SHADER);
            GLuint fragment = create_shader(fragment_path, GL_FRAGMENT_SHADER);
            this->ID = glCreateProgram();
            glCompileShader(vertex);
            GLint success;
            glAttachShader(this->ID, vertex);
            glAttachShader(this->ID,fragment);
            glLinkProgram(this->ID);
            glGetProgramiv(this->ID,GL_LINK_STATUS, &success);
            if (!success){
                cerr<<"Error linking program"<<endl;
            }
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        void Shader:: use(){
            glUseProgram(this->ID);
        }
        void Shader::set_bool(const string &name, bool value){
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform1i(glGetUniformLocation(this->ID,name.c_str()),value);
        }
        void Shader::set_int(const string &name, int value){
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform1i(glGetUniformLocation(this->ID,name.c_str()),value);
        }
        void Shader::set_float(const string &name, float value){
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform1f(glGetUniformLocation(this->ID,name.c_str()),value);
        }
        void Shader::set_sampler(const string &name, int unit){
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform1i(glGetUniformLocation(this->ID,name.c_str()),unit);
        }
        void Shader::set_vector2f(const string &name, float x, float y, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform2f(glGetUniformLocation(this->ID,name.c_str()),x,y);
        } 
        void Shader::set_vector2f(const string &name, glm::vec2 vals, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform2f(glGetUniformLocation(this->ID,name.c_str()),vals.x,vals.y);
        } 
        void Shader::set_vector3f(const string &name, float x, float y,float z, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform3f(glGetUniformLocation(this->ID,name.c_str()),x,y,z);
        } 
        void Shader::set_vector3f(const string &name, glm::vec3 vals, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform3f(glGetUniformLocation(this->ID,name.c_str()),vals.x,vals.y,vals.z);
        } 
        void Shader::set_vector4f(const string &name, float x, float y, float z, float w, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform4f(glGetUniformLocation(this->ID,name.c_str()),x,y,z,w);
        } 
        void Shader::set_vector4f(const string &name, glm::vec4 vals, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniform4f(glGetUniformLocation(this->ID,name.c_str()),vals.x,vals.y,vals.z,vals.w);
        } 
        void Shader::set_matrix4(const string &name, glm::mat4 matrix, bool use_shader){
            if (use_shader){
                use();
            }
            GLint loc = glGetUniformLocation(this->ID,name.c_str());
            if (loc == -1){
                cerr<<"Failed to bind uniform: "<<name<<endl;
            }
            glUniformMatrix4fv(glGetUniformLocation(this->ID,name.c_str()),1,false,glm::value_ptr(matrix));
        } 
        GLint Shader::attach_attribute(const string &name){
            GLint attribute = glGetAttribLocation(this->ID,name.c_str());
            if (attribute == -1){
                cerr<<"Error attaching attribute "<<name;
                return -1;
            }
            return attribute;
        }