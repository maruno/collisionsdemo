#version 150 core

in vec3 colour;
out vec4 fragColour;

void main(void) {
	fragColour = vec4(colour, 1.);
}