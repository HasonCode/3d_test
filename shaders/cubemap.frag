#version 120
varying vec3 tex_cords;
uniform samplerCube cubemap;
uniform vec3 color;
uniform vec3 camera_pos;
uniform vec3 light_color;
varying vec3 normal;
varying vec3 frag_pos;

struct Material{
    vec3 ambience;
    vec3 specular;
    vec3 diffuse;
    float shininess;
};

struct Light{
    vec3 position;

    vec3 diffuse;
    vec3 specular;
    vec3 ambience;
};

uniform Light light;
uniform Material material;

void main(){
    // Ambient lighting
    vec3 ambient = material.ambience * light.ambience;

    float spec_strength = 0.5;
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 view_direction = normalize(camera_pos - frag_pos);
    // Diffuse lighting
    vec3 norm = normalize(normal);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse =  light.diffuse * (diff * material.diffuse);

    float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.diffuse;
    vec3 result = (ambient + diffuse + specular) * color;
    gl_FragColor = vec4(result, 1.0) * textureCube(cubemap, tex_cords);
}