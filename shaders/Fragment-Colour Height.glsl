#version 150 core

in vec3 colour;
in float z;

out vec4 fragColour;

void main(void) {
	fragColour = vec4(colour, 1.);
	
	gl_FragDepth = z;
}