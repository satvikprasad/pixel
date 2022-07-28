#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 v_frag_pos;
out vec3 v_normal;

uniform mat4 m, v, p;

void main() {
	v_frag_pos = vec3(m * vec4(position, 1.0));
    v_normal = mat3(transpose(inverse(m))) * normal;
    gl_Position = p * v * vec4(v_frag_pos, 1.0);
}
