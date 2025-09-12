#version 120

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

attribute vec3 a_pos;
attribute vec3 a_normal;
varying vec3 frag_pos;
varying vec3 normal;
varying vec3 tex_cords;
void main(){
    tex_cords = a_pos;
    normal = a_normal;
    frag_pos = vec3(model * vec4(a_pos, 1.0));
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
}