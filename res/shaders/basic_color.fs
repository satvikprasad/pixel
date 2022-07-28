#version 330

uniform vec4 color;

out vec4 frag_color;

void main() {
    // gamma correction
    float gamma = 2.2;
    frag_color = pow(color, vec4(1.0/gamma));
}
