#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D woodTexture;

void main() {
	FragColor = texture(woodTexture, TexCoord);
}