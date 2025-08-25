#version 120
uniform mat4 model;
uniform mat4 projection;
attribute vec2 coordt;
attribute vec2 coord2d;
varying vec2 tex_coord;
void main(void){
    gl_Position = projection*model*vec4(coord2d,0.0,1.0);
    tex_coord = coordt;
}