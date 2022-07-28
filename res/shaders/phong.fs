#version 330

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shine;
};
uniform Material material;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dir_light;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define MAX_POINT_LIGHTS 5
uniform PointLight point_lights[MAX_POINT_LIGHTS];

out vec4 frag_color;

in vec3 v_normal;
in vec3 v_frag_pos;

uniform vec3 view_pos;
uniform int point_light_count;

vec3 calc_dir_light(DirLight self, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight self, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main() {
    // properties
    vec3 norm = normalize(v_normal);
    vec3 view_dir = normalize(view_pos - v_frag_pos);

    vec3 result = calc_dir_light(dir_light, norm, view_dir);
    
    for (int i = 0; i < point_light_count; ++i) 
        result += calc_point_light(point_lights[i], norm, v_frag_pos, view_dir);

    // gamma correction
    float gamma = 2.2;
    frag_color = vec4(pow(result, vec3(1.0/gamma)), 1.0);
}

vec3 calc_dir_light(DirLight self, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-self.direction);

    float diff = max(dot(normal, light_dir), 0.0);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(view_dir, halfway_dir), 0.0), material.shine);

    vec3 ambient = self.ambient * material.diffuse;
    vec3 diffuse = self.diffuse * diff * material.diffuse;
    vec3 specular = self.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight self, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    float distance = length(self.position - v_frag_pos);
    float attenuation = 1.0 / (self.constant + self.linear * distance + self.quadratic * (distance * distance));

    // Diffuse
    vec3 light_dir = normalize(self.position - v_frag_pos); 
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float intensity = dot(normal, light_dir);
    float diff = max(intensity, 0.0f);

    // Specular
    float spec = pow(max(dot(view_dir, halfway_dir), 0.0), material.shine);

    vec3 ambient = self.ambient * material.diffuse * attenuation;
    vec3 diffuse = self.diffuse * (diff * material.diffuse) * attenuation;
    vec3 specular = self.specular * (spec * material.specular) * attenuation;
    return ambient + diffuse + specular;
}
