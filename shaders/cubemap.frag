#version 120
varying vec3 tex_cords;
uniform samplerCube cubemap;
uniform vec3 color;
uniform vec3 camera_pos;
uniform vec3 light_color;
varying vec3 normal;
varying vec3 frag_pos;

struct Material{
    samplerCube diffuse;
    samplerCube specular;
    float shininess;
};

struct DirLight{
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
    vec3 ambience;
};
uniform DirLight dir_light;

struct PointLight{
    vec3 position;

    vec3 diffuse;
    vec3 specular;
    vec3 ambience;

    float constant;
    float linear;
    float quadratic;
};
#define num_point_lights 1

uniform PointLight point_lights[num_point_lights];
uniform Material material;

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cut_off;

    vec3 diffuse;
    vec3 specular;
    vec3 ambience;

    float constant;
    float linear;
    float quadratic;
};
#define num_spot_lights 1
uniform SpotLight spot_lights[num_spot_lights];

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_direction){
    vec3 light_dir = normalize(-light.direction);
    
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureCube(material.diffuse, tex_cords).rgb;
    vec3 reflect_dir = reflect(-light_dir, normal);

    float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * textureCube(material.diffuse, tex_cords).rgb;

    vec3 ambience = light.ambience * textureCube(material.diffuse, tex_cords).rgb;
    return (diffuse + specular + ambience);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_direction){
    vec3 light_dir = normalize(light.position - frag_pos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureCube(material.diffuse, tex_cords).rgb;
    vec3 reflect_dir = reflect(-light_dir, normal);

    float dist = length(light.position - frag_pos);
    float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

    diffuse*=attenuation;

    float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * textureCube(material.diffuse, tex_cords).rgb;

    vec3 ambience = light.ambience * textureCube(material.diffuse, tex_cords).rgb;
    
    specular*=attenuation;
    ambience*=attenuation;
    return (diffuse + specular + ambience);
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_direction){
    vec3 light_dir = normalize(light.position - frag_pos);
    float dist = length(light.position - frag_pos);
    float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);
    float angle = dot(light_dir, normalize(-light.direction));
    if (angle > light.cut_off){
        float diff = max(dot(norm, light_dir), 0.0);
        vec3 diffuse = light.diffuse * diff * textureCube(material.diffuse, tex_cords).rgb;
        vec3 reflect_dir = reflect(-light_dir, normal);


        diffuse*=attenuation;

        float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * textureCube(material.diffuse, tex_cords).rgb;

        vec3 ambience = light.ambience * textureCube(material.diffuse, tex_cords).rgb;
        
        specular*=attenuation;
        ambience*=attenuation;
        return (diffuse + specular + ambience);
    }
    return light.ambience * textureCube(material.diffuse, tex_cords).rgb * attenuation;
    
}

void main(){



    // Ambient lighting
    vec3 norm = normalize(normal);
    float spec_strength = 0.5;
    vec3 view_direction = normalize(camera_pos - frag_pos);
    vec3 result = calc_dir_light(dir_light, norm, view_direction);
    for (int i = 0; i < num_point_lights; i++){
        result+=calc_point_light(point_lights[i], norm, frag_pos, view_direction);
    }
    for (int i = 0; i < num_spot_lights; i++){
        result+=calc_spot_light(spot_lights[i], norm, frag_pos, view_direction);
    }
    result *= color;
    gl_FragColor = vec4(result, 1.0) * textureCube(cubemap, tex_cords);
}