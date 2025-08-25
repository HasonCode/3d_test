#version 120
varying vec2 tex_coord;
uniform vec3 sprite_color;
uniform sampler2D image;
void main(void){
    gl_FragColor = vec4(sprite_color,1.0) * texture2D(image,tex_coord);

}