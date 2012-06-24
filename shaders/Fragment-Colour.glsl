#version 150 core

layout(std140) uniform;

uniform colourUni {
	vec3 colour;
};

out vec4 fragColour;

void main(void) {
	fragColour = vec4(colour, 1.);
}