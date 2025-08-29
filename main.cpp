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
#include "SpriteRenderer.h"
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
int screen_width= 1600, screen_height=2000;
//Window
SDL_Window* window;

//Texture

float vert_vel = 0.0;
float vert_acc = 0.0;

struct block{
    int x, y, z;
    int type;
};

vector<string> textures;
vector<block> blocks;

void render (SDL_Window* window, Shader s, CubeRenderer renderer, vector<block> blocks){
    glClearColor(0.7,0.7,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    s.use();
    int x = 0, y = 0, z = 0;
    glm::vec3 scale = glm::vec3(5, 5, 5);
    glm::vec3 rotation = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
    Texture3D texture = ResourceManager::get_texture("testing_cube");
    for (int i = 0; i < blocks.size(); i++){
        texture = ResourceManager::get_texture(textures[blocks[i].type]);
        renderer.drawCube(texture, glm::vec3(blocks[i].x, blocks[i].y, blocks[i].z), scale, rotation, color);
    }
    renderer.drawCube(texture, glm::vec3(x, y, z), scale, rotation, color);
}

void render_gui(SDL_Window* window, Shader s, SpriteRenderer renderer){
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    s.use();
    //Selector
    int x = screen_width/2, y = screen_height/2;
    glm::vec2 size = glm::vec2(20,20);
    float rotate = 0.0;
    glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
    Texture2D sprite = ResourceManager::get_sprite("selector");
    renderer.DrawSprite(sprite, glm::vec2(x, y), size, rotate, color);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
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

CubeRenderer setup(Shader& shader, Camera camera){
    CubeRenderer renderer(shader, camera);
    return renderer;
}

float* movement_vector(vector<float> cam_pos, vector<float> look_at, float move_amount, float rotation, float rotation2){
    float* ret_arr = (float*)malloc(3*sizeof(float));
    float x = abs(cam_pos[0]-look_at[0]);
    float y = abs(cam_pos[1]-look_at[2]);
    float z = abs(cam_pos[2]-look_at[2]);
    float dist = sqrt(pow(x,2.0)+pow(y,2.0)+pow(z,2.0));
    ret_arr[0] = move_amount*cos(rotation)*cos(rotation2);
    ret_arr[1] = move_amount * sin(rotation2);
    ret_arr[2] = move_amount * sin(rotation)*cos(rotation2);
    if (cam_pos[0]>look_at[0]){
        cam_pos[0]*=-1;
    }
    if (cam_pos[1]>look_at[1]){
        cam_pos[1]*=-1;
    }
    if (cam_pos[1]>look_at[1]){
        cam_pos[1]*=-1;
    }
    return ret_arr;
}

bool above_block(CubeRenderer renderer, vector<block> blocks){
    // cout<<"Camera y: "<<renderer.camera.position.y<<endl;
    for (int i = 0; i < blocks.size(); i++){
        if (abs(blocks[i].x -renderer.camera.position.x)<=5 && abs(blocks[i].z -renderer.camera.position.z)<=5 && abs(blocks[i].y + -renderer.camera.position.y)<=25){
            return true;
        }
    }
    return false;
}    

bool key_matters(SDL_Event ev, vector<bool> keys){
    if (ev.type == SDL_KEYDOWN){
        if (ev.key.keysym.scancode == SDL_SCANCODE_W && keys[0]){
            return false;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_S && keys[1]){
            return false;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_A && keys[2]){
            return false;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_D && keys[3]){
            return false;
        }
    }
    return true;
}

void main_loop(SDL_Window* window, Shader s, Shader s2, CubeRenderer renderer, SpriteRenderer spriter){
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetWindowGrab(window, SDL_TRUE);
    bool flag = true;
    int mouse_state = 0;
    int mousex, mousey;
    int prev_x, prev_y;
    SDL_GetMouseState(&mousex, &mousey);
    SDL_GetMouseState(&prev_x, &prev_y);
    float rotation = 1.67, rotation2 = 1.67;
    float look_dist =20, turn_speed = 0.01;
    float lookx=5, looky=5, lookz=5, facex=20, facey=30, facez=20;
    bool grounded = false;
    int move_dist = 1;
    bool a_down = false, s_down = false, d_down = false, w_down = false;
    while(true){
        SDL_Event ev;
        grounded = above_block(renderer, blocks);
        if (!grounded){
            vert_acc = -20;
        }
        else if (vert_vel<0){
            vert_vel = 0.0;
            vert_acc = 0.0;
        }
        vert_vel += vert_acc * 1/60.0;
        facey+=vert_vel * 1/60.0;
        looky+=vert_vel * 1/60.0;
        SDL_GetMouseState(&mousex, &mousey);
        int delt_x = mousex - prev_x; 
        int delt_y = mousey - prev_y;
        prev_x = mousex;
        prev_y = mousey;
        rotation -= delt_x * turn_speed;
        rotation2 -= delt_y * turn_speed;
        // just to see (imagine if)
        if (rotation2>1.57){
            rotation2 = 1.57;
        }
        else if (rotation2<-1.57){
            rotation2 = -1.57;
        }
        lookx = -look_dist * cos(rotation) * cos(rotation2) + facex;
        lookz = look_dist * sin(rotation) * cos(rotation2) + facez;
        looky = look_dist * sin(rotation2) + facey;
        renderer.camera.change_target(lookx, looky, lookz);
        if (s_down){
            glm::vec3 camera_pos = glm::vec3(facex, facey, facez);
            glm::vec3 camera_target = glm::vec3(lookx, looky, lookz);
            glm::vec3 forward = glm::normalize(camera_pos - camera_target);
            float x_move = forward.x*move_dist;
            float z_move = forward.z*move_dist;
            facex+=x_move;
            facez+=z_move;
            lookx+=x_move;
            lookz+=z_move;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (w_down){
            glm::vec3 camera_pos = glm::vec3(facex, facey, facez);
            glm::vec3 camera_target = glm::vec3(lookx, looky, lookz);
            glm::vec3 forward = glm::normalize(camera_pos - camera_target);
            float x_move = forward.x * move_dist;
            float z_move = forward.z*move_dist;
            facex-=x_move;
            facez-=z_move;
            lookx-=x_move;
            lookz-=z_move;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (d_down){
            vector<float> cam_pos = {facex, facey, facez};
            vector<float> look_at = {lookx, looky, lookz};
            float z_move = cos(rotation)*move_dist;
            float x_move = sin(rotation)*move_dist;
            facex-=x_move;
            facez-=z_move;
            lookx-=x_move;
            lookz-=z_move;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (a_down){
            vector<float> cam_pos = {facex, facey, facez};
            vector<float> look_at = {lookx, looky, lookz};
            float z_move = cos(rotation)*move_dist;
            float x_move = sin(rotation)*move_dist;
            facex+=x_move;
            facez+=z_move;
            lookx+=x_move;
            lookz+=z_move;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        while(SDL_PollEvent(&ev) && key_matters(ev, {w_down, s_down, a_down, d_down})){
            if(ev.type==SDL_QUIT){
                return;
            }
            if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                on_resize(ev.window.data1,ev.window.data2);
            }
            if (ev.type == SDL_KEYDOWN){
                if (ev.key.keysym.scancode == SDL_SCANCODE_W){
                    w_down = true;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_S){
                    s_down = true;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_D){
                    d_down = true;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_A){
                    a_down = true;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_SPACE){
                    if (grounded){
                        vert_vel = 15.0;
                    }
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_R){
                    lookx=5, looky=5, lookz=5, facex=20, facey=30, facez=20;
                }
            }
            if (ev.type == SDL_KEYUP){
                if (ev.key.keysym.scancode == SDL_SCANCODE_W){
                    w_down = false;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_S){
                    s_down = false;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_A){
                    a_down = false;
                }
                if (ev.key.keysym.scancode == SDL_SCANCODE_D){
                    d_down = false;
                }
            }
            if (ev.type == SDL_MOUSEBUTTONDOWN){
                mouse_state = SDL_GetMouseState(&mousex, &mousey);
                if (ev.button.button == SDL_BUTTON_LEFT && !flag){
                    flag = true;
                    prev_x = ev.button.x;
                    prev_y = ev.button.y;
                    mousex = ev.button.x;
                    mousey = ev.button.y;
                }
            }
        }
        renderer.camera.change_target(lookx, looky, lookz);
        renderer.camera.move_camera(facex, facey, facez);
        logic(s);
        render(window,s, renderer, blocks);
        // SDL_GL_SwapWindow(window);
        render_gui(window, s2, spriter);
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
    Camera camera(glm::vec3(20, 35, 20), glm::vec3(5, 5, 5));
    camera.update_screen(screen_height, screen_width);
    Shader shader("shaders/cubemap.vert", "shaders/cubemap.frag");

    CubeRenderer renderer(shader, camera);
    ResourceManager::load_texture("sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "dirt");
    ResourceManager::load_texture("sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/grass_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "sprites/dirt_block.png", "grass");
    ResourceManager::load_texture("sprites/stone_block.png", "sprites/stone_block.png", "sprites/stone_block.png", "sprites/stone_block.png", "sprites/stone_block.png", "sprites/stone_block.png", "stone");
    ResourceManager::load_texture("sprites/cobblestone.png", "sprites/cobblestone.png", "sprites/cobblestone.png", "sprites/cobblestone.png", "sprites/cobblestone.png", "sprites/cobblestone.png", "cobblestone");
    ResourceManager::load_texture("sprites/wood_block.png", "sprites/wood_block.png", "sprites/wood_innards.png", "sprites/wood_innards.png", "sprites/wood_block.png", "sprites/wood_block.png", "wood");
    ResourceManager::load_texture("sprites/leaves.png", "sprites/leaves.png", "sprites/leaves.png", "sprites/leaves.png", "sprites/leaves.png", "sprites/leaves.png", "leaves");
    textures = {"dirt", "grass", "stone", "cobblestone", "wood", "leaves"};
    ResourceManager::load_shader("shaders/cubemap.vert", "shaders/cubemap.frag", "cubemap");
    ResourceManager::load_shader("shaders/sprite.vert", "shaders/sprite.frag", "sprite");
    Shader s2 = ResourceManager::get_shader("sprite");
    ResourceManager::load_sprite("sprites/selector.png", "selector");
    Texture2D sprite = ResourceManager::get_sprite("selector");
    SpriteRenderer spriter(s2);
    blocks = {block{0,0,0,0}, block{10,0,0,1}, block{20,0,0,2}, block{30,0,0,3}, block{40,0,0,4}, block{50,0,0,5}};
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 6; j++){
            blocks.push_back(block{j*10, 0, (i+1)*10, j});
        }
    }
    glViewport(0,0,screen_width,screen_height);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,1);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    main_loop(window, shader, s2, renderer, spriter);
    return EXIT_SUCCESS;
}