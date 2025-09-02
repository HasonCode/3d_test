#include "SpriteRenderer.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// #include "shader_utils.h"
#include "Shader.h"
#include "Texture2D.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include "stb_image.h"
SpriteRenderer:: SpriteRenderer(){
    shader = Shader();
}
SpriteRenderer::SpriteRenderer(Shader &shader): shader(shader){
    this->initRenderData();
    this->shader.use();
    this->shader.set_sampler("image",0);
}
SpriteRenderer::~SpriteRenderer(){
    glDeleteVertexArrays(1,&this->quadVAO);
}
void SpriteRenderer::initRenderData(){
    float vertices[] = {
        //pos       //tex
        0.0, 1.0, 0.0, 1.0,
        1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0,

        0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0
    };
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    this->coord2d = shader.attach_attribute("coord2d");
    this->textt = shader.attach_attribute("coordt");
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glEnableVertexAttribArray(this->coord2d);
    glVertexAttribPointer(
        this->coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        4*sizeof(float),
        0
    );
    glVertexAttribPointer(
        this->textt,
        2,
        GL_FLOAT,
        GL_FALSE,
        4*sizeof(float),
        (GLvoid*)(2 * sizeof(GLfloat))
    );
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position,
    glm::vec2 size, float rotate, glm::vec3 color, float height, float width){
        this->shader.use();
        this->shader.set_sampler("image",0);
        glm::mat4 projection = glm::ortho(0.0f,width,height,0.0f,-1.0f,1.0f); 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(position,0.0f));

        model = glm::translate(model,glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model,rotate,glm::vec3(0.0f,0.0f,1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

        model = glm::scale(model,glm::vec3(size,1.0));
        this->shader.set_matrix4("projection",projection,true);
        this->shader.set_matrix4("model",model,true);
        this->shader.set_vector3f("sprite_color",color,true);
        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
        glEnableVertexAttribArray(this->textt);
        glEnableVertexAttribArray(this->coord2d);
        glVertexAttribPointer(
            this->coord2d,
            2,
            GL_FLOAT,
            GL_FALSE,
            4*sizeof(float),
            0
        );
        glVertexAttribPointer(
            this->textt,
            2,
            GL_FLOAT,
            GL_FALSE,
            4*sizeof(float),
            (GLvoid*)(2 * sizeof(GLfloat))
        );
        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES,0,6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        this->shader.use();
    }