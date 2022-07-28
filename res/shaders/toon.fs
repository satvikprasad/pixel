#version 330

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shine;
};
uniform Material material;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight point_light;

uniform float n_shades;
uniform vec3 view_pos;

out vec4 frag_color;

in vec3 v_normal;
in vec3 v_frag_pos;

void main() {
    float distance = length(point_light.position - v_frag_pos);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));

    // Ambient
    float ambient_strength = 0.4;
    vec3 ambient = material.diffuse * point_light.ambient * attenuation;

    // Diffuse
    vec3 norm = normalize(v_normal);
    vec3 light_dir = normalize(point_light.position - v_frag_pos);
    float step = 100/n_shades;
    float intensity = dot(norm, light_dir);
    float diff = max(intensity, 0.f);
    diff = int(diff * 100/step)*(step/100.0);

    vec3 diffuse = point_light.diffuse * (diff * material.diffuse) * attenuation;

    // Specular
    vec3 view_dir = normalize(view_pos - v_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shine);
    float spec_step = step*2;
    spec = int(spec * 100/spec_step)*(spec_step/100.0);
    vec3 specular = point_light.specular * (spec * material.specular) * attenuation;
    
    vec3 result = (ambient + diffuse + specular);
    frag_color = vec4(result, 1.0);
}
