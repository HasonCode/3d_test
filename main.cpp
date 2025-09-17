/*TO DO: 
POLISH UP MAIN MAP FOR PARKOUR
ADD IN REAL SHADING AND LIGHTING*/
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
#include "AABB.h"
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
int screen_width= 1920, screen_height=1080;
//Window
SDL_Window* window;

//AABB Manager
AABBManager manager;

//Texture

float vert_vel = 0.0;
float vert_acc = 0.0;

struct block{
    int x, y, z;
    int type;
    AABB aabb;
};

enum{
    DIRT=0,
    GRASS=1,
    STONE=2,
    COBBLESTONE=3,
    WOOD=4,
    LEAVES=5,
    MOSS=6,
    GEM=7,
};

vector<string> textures;
vector<block> blocks;
float theta = 0.0;
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
        renderer.drawCube(texture, glm::vec3(blocks[i].x*10, blocks[i].y*10, blocks[i].z*10), scale, rotation, color, glm::vec3(30,30,30));
    }
    renderer.drawCube(texture, glm::vec3(x, y, z), scale, rotation, color);
    Texture3D light_texture = ResourceManager::get_texture("white_square");
    renderer.drawCube(light_texture, glm::vec3(30,30,30), glm::vec3(1,1,1),rotation, glm::vec3(1.0, 1.0, 1.0), glm::vec3(30,30,30));
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
    renderer.DrawSprite(sprite, glm::vec2(x, y), size, rotate, color, screen_height, screen_width);
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

void render_screen(SDL_Window* window, Shader s, SpriteRenderer renderer, Texture2D sprite){
    while(true){
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            if(ev.type==SDL_QUIT){
                return;
            }
            if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                on_resize(ev.window.data1,ev.window.data2);
            }
            if (ev.type == SDL_KEYDOWN){
                if (ev.key.keysym.scancode == SDL_SCANCODE_RETURN){
                    return;
                }
            }
        }
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        s.use();
        renderer.DrawSprite(sprite, glm::vec2(0, 0), glm::vec2(screen_width, screen_height), 0.0, glm::vec3(1.0, 1.0, 1.0), screen_height, screen_width);
        SDL_GL_SwapWindow(window);
    }
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

vector<bool> above_block(CubeRenderer renderer, vector<block> blocks, AABB player){
    vector<bool> ret_arr = vector<bool>(2);
    ret_arr[0] = false;
    ret_arr[1] = false;
    float midx = (player.min.x+player.max.x)/2.0;
    float midz = (player.min.z+player.max.z)/2.0;
    AABB mod_player = AABB{glm::vec3(midx-4, player.min.y-0.2f, midz-4), glm::vec3(midx+4, player.max.y, midz+4)};
    for (int i = 0; i < blocks.size(); i++){
        if (AABBManager::check_intersect(mod_player, blocks[i].aabb)){
            ret_arr[0] = true;
            ret_arr[1] = false;
            if (blocks[i].type == GEM){
                ret_arr[1] = true;
            } 
        }
    }
    return ret_arr;
}    

glm::vec3 eject_from_blocks(CubeRenderer renderer, glm::vec3 position, glm::vec3 player_size){
    glm::vec3 player_min = glm::vec3(position.x, position.y - player_size.y+3, position.z);
    glm::vec3 player_max = glm::vec3(position.x+player_size.x, position.y, position.z+player_size.z);
    AABB player_box = AABB{player_min, player_max};
    AABB collide;
    for (int i = 0; i < manager.aabbs.size(); i++){
        if (AABBManager::check_intersect(player_box, manager.aabbs[i])){
            collide = manager.aabbs[i];
            float eject_x = min(player_box.max.x - collide.min.x, collide.max.x - player_box.min.x);
            float eject_z = min(player_box.max.z - collide.min.z, collide.max.z - player_box.min.z);
            glm::vec3 player_center = (player_box.min + player_box.max) * 0.5f;
            glm::vec3 block_center = (collide.min + collide.max) * 0.5f;
            if (abs(eject_x) < abs(eject_z)){
                if (player_center.x < block_center.x){
                    position.x -= eject_x;
                }
                else{
                    position.x += eject_x;
                }
            }
            else{
                if (player_center.z < block_center.z){
                    position.z -= eject_z;
                }
                else{
                    position.z += eject_z;
                }
            }
        }
    }
    return position;
    
}
glm::vec3 block_in_direction(CubeRenderer renderer, vector<block> blocks, glm::vec3 movement, AABB player){
    AABB tempx = AABB{glm::vec3(movement.x,2,0)+player.min, glm::vec3(movement.x,2,0)+player.max};
    AABB tempz = AABB{glm::vec3(0,2,movement.z)+player.min, glm::vec3(0,2,movement.z)+player.max};
    if (manager.check_collision(tempx)){
        movement.x = 0;
    }
    if (manager.check_collision(tempz)){
        movement.z = 0;
    }
    return movement;
}

bool key_matters(SDL_Event ev){
    if (ev.type == SDL_KEYDOWN){
        if (ev.key.keysym.scancode == SDL_SCANCODE_W){
            return false;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_S){
            return false;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_A){
            return false;
        }
        if (ev.key.keysym.scancode == SDL_SCANCODE_D){
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
    float rotation = -1, rotation2 = 0;
    float look_dist =20, turn_speed = 0.01;
    float lookx=0, looky=2, lookz=0, facex=20, facey=50, facez=20;
    AABB player = AABB{glm::vec3(facex, facey-19, facez), glm::vec3(facex+10, facey, facez+10)};
    vector<bool> state_arr;
    bool grounded = true;
    bool win = false;
    float move_dist = 0.75;
    //JUMP
    float down_acc = -120;
    float jump_height = 55;
    const  Uint8* keystates = SDL_GetKeyboardState(NULL);
    while(true){
        player = AABB{glm::vec3(facex, facey-18.9, facez), glm::vec3(facex+10, facey, facez+10)};
        SDL_Event ev;
        state_arr = above_block(renderer, blocks, player);
        grounded = state_arr[0];
        win = state_arr[1];
        if (!grounded){
            vert_acc = down_acc;
        }
        else if (vert_vel<0){
            vert_vel = 0.0;
            vert_acc = 0.0;
        }
        vert_vel += vert_acc * 1/60.0;
        facey+=vert_vel * 1/60.0;
        looky+=vert_vel * 1/60.0;
        theta+=1/60.0;
        SDL_GetMouseState(&mousex, &mousey);
        int delt_x = mousex - prev_x; 
        int delt_y = mousey - prev_y;
        prev_x = mousex;
        prev_y = mousey;
        rotation -= delt_x * turn_speed;
        rotation2 -= delt_y * turn_speed;
        rotation2 = glm::clamp(rotation2, -1.60f, 1.60f);
        lookx = -look_dist * cos(rotation) * cos(rotation2) + facex;
        lookz = look_dist * sin(rotation) * cos(rotation2) + facez;
        looky = look_dist * sin(rotation2) + facey;
        renderer.camera.change_target(lookx, looky, lookz);
        if (keystates[SDL_SCANCODE_SPACE]){
            if (grounded){
                vert_vel = jump_height;
            }
        }
        if (keystates[SDL_SCANCODE_R]){
            lookx=0, looky=2, lookz=0, facex=20, facey=50, facez=20;
            vert_vel = 0;
        }
        if (keystates[SDL_SCANCODE_S]){
            glm::vec3 camera_pos = glm::vec3(facex, facey, facez);
            glm::vec3 camera_target = glm::vec3(lookx, facey, lookz);
            glm::vec3 forward = glm::normalize(camera_pos - camera_target);
            forward = block_in_direction(renderer, blocks, forward, player);
            float x_move = forward.x*move_dist;
            float z_move = forward.z*move_dist;
            facex+=x_move;
            facez+=z_move;
            lookx+=x_move;
            lookz+=z_move;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (keystates[SDL_SCANCODE_W]){
            glm::vec3 camera_pos = glm::vec3(facex, facey, facez);
            glm::vec3 camera_target = glm::vec3(lookx, facey, lookz);
            glm::vec3 forward = glm::normalize(camera_pos - camera_target);
            forward.x*=move_dist;
            forward.z*=move_dist;
            forward = block_in_direction(renderer, blocks, forward, player);
            facex-=forward.x;
            facez-=forward.z;
            lookx-=forward.x;
            lookz-=forward.z;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (keystates[SDL_SCANCODE_D]){
            glm::vec3 cam_pos = glm::vec3(facex, looky, facez);
            glm::vec3 look_at = glm::vec3(lookx, looky, lookz);
            glm::vec3 up = glm::vec3(0,1,0);
            glm::vec3 diff = glm::normalize(cam_pos-look_at);
            glm::vec3 forward = glm::normalize(glm::cross(diff,up));
            forward.x*=move_dist;
            forward.z*=move_dist;
            forward = block_in_direction(renderer, blocks, forward, player);
            facex-=forward.x;
            facez-=forward.z;
            lookx-=forward.x;
            lookz-=forward.z;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (keystates[SDL_SCANCODE_A]){
            glm::vec3 cam_pos = glm::vec3(facex, looky, facez);
            glm::vec3 look_at = glm::vec3(lookx, looky, lookz);
            glm::vec3 up = glm::vec3(0,1,0);
            glm::vec3 diff = glm::normalize(cam_pos-look_at);
            glm::vec3 forward = glm::normalize(glm::cross(diff,up));
            forward.x*=move_dist;
            forward.z*=move_dist;
            forward = block_in_direction(renderer, blocks, forward, player);
            facex+=forward.x;
            facez+=forward.z;
            lookx+=forward.x;
            lookz+=forward.z;
            renderer.camera.change_target(lookx, looky, lookz);
            renderer.camera.move_camera(facex, facey, facez);
        }
        if (keystates[SDL_SCANCODE_ESCAPE]){
            return;
        }
        while(SDL_PollEvent(&ev) && key_matters(ev)){
            if(ev.type==SDL_QUIT){
                return;
            }
            if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                on_resize(ev.window.data1,ev.window.data2);
            }
            
        }
        glm::vec3 current_pos = glm::vec3(facex, facey, facez);
        glm::vec3 ejection = eject_from_blocks(renderer, current_pos, glm::vec3(10, 20, 10));
        if (ejection != current_pos){
            float delt_x = ejection.x - facex;
            float delt_z = ejection.z - facez;
            facex += delt_x;
            facez += delt_z;
            lookx += delt_x;
            lookz += delt_z;
        }
        renderer.camera.change_target(lookx, looky, lookz);
        renderer.camera.move_camera(facex, facey, facez);
        if (win){
            render_screen(window, s2, spriter, ResourceManager::get_sprite("winscreen"));
            return;
        }
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


block make_block(int x, int y, int z, int type){
    AABB aabb;
    aabb.min = glm::vec3(x*10, y*10, z*10);
    aabb.max = glm::vec3(x*10+10, y*10+10, z*10+10);
    manager.add_aabb(aabb);
    return block{x, y, z, type, aabb};
}
int main(int argc, char* argv[]){
    SDL_SetRelativeMouseMode(SDL_TRUE);
    manager = AABBManager();
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
    window = SDL_CreateWindow("It's hopping time out here",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screen_width,screen_height,
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
    ResourceManager::load_texture("sprites/mossy_cobble.png", "sprites/mossy_cobble.png", "sprites/mossy_cobble.png", "sprites/mossy_cobble.png", "sprites/mossy_cobble.png", "sprites/mossy_cobble.png", "mossy_cobble");
    ResourceManager::load_texture("sprites/gem_block.png", "sprites/gem_block.png", "sprites/gem_block.png", "sprites/gem_block.png", "sprites/gem_block.png", "sprites/gem_block.png", "gem_block");
    ResourceManager::load_texture("sprites/white_square.png", "sprites/white_square.png", "sprites/white_square.png", "sprites/white_square.png", "sprites/white_square.png", "sprites/white_square.png", "white_square");
    textures = {"dirt", "grass", "stone", "cobblestone", "wood", "leaves", "mossy_cobble", "gem_block"};
    ResourceManager::load_shader("shaders/cubemap.vert", "shaders/cubemap.frag", "cubemap");
    ResourceManager::load_shader("shaders/sprite.vert", "shaders/sprite.frag", "sprite");
    Shader s2 = ResourceManager::get_shader("sprite");
    ResourceManager::load_sprite("sprites/selector.png", "selector");
    ResourceManager::load_sprite("sprites/winscreen.png", "winscreen");
    ResourceManager::load_sprite("sprites/startscreen.png", "startscreen");
    Texture2D sprite = ResourceManager::get_sprite("selector");
    SpriteRenderer spriter(s2);
    blocks = {make_block(0,0,0,DIRT), make_block(1,0,0,GRASS), make_block(2,0,0,STONE), make_block(3,0,0,COBBLESTONE), make_block(4,0,0,WOOD), make_block(5,0,0,LEAVES)};
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 6; j++){
            blocks.push_back(make_block(j, 0, (i+1), j));
        }
    }
    blocks.push_back(make_block(4,1,4,0));

    for (int i = 0; i < 10; i++){
        blocks.push_back(make_block(5*i+13, i, 4, MOSS));
        blocks.push_back(make_block(5*i+13, i, 5, MOSS));
        blocks.push_back(make_block(5*i+13, i, 6, MOSS));
        blocks.push_back(make_block(5*i+12, i, 4, MOSS));
        blocks.push_back(make_block(5*i+12, i, 5, MOSS));
        blocks.push_back(make_block(5*i+12, i, 6, MOSS));
        blocks.push_back(make_block(5*i+14, i, 4, MOSS));
        blocks.push_back(make_block(5*i+14, i, 5, MOSS));
        blocks.push_back(make_block(5*i+14, i, 6, MOSS));

    }
    blocks.push_back(make_block(63, 9, 8, MOSS));

    blocks.push_back(make_block(65, 10, 10, MOSS));

    blocks.push_back(make_block(65, 11, 14, MOSS));

    blocks.push_back(make_block(65, 12, 18, MOSS));

    blocks.push_back(make_block(66, 13, 21, MOSS));

    blocks.push_back(make_block(66, 13, 25, GEM));

    
    glViewport(0,0,screen_width,screen_height);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,1);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    render_screen(window, s2, spriter, ResourceManager::get_sprite("startscreen"));
    main_loop(window, shader, s2, renderer, spriter);
    free_resources();
    return EXIT_SUCCESS;
}