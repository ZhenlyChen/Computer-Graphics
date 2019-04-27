#version 450 core
out vec4 FragColor;


in vec3 outColor;

uniform vec3 objectColor;

void main() {
    FragColor = vec4(outColor * objectColor, 1.0);
}