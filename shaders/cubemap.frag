#version 120
varying vec3 tex_cords;
uniform samplerCube cubemap;
uniform vec3 color;

void main(){
    gl_FragColor = vec4(color, 1.0) * textureCube(cubemap, tex_cords);
}