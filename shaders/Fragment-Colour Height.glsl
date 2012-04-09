#version 150 core

in float y;
out vec4 fragColour;

void main(void) {
	vec3 colour;
	
	if(y < 0.0) {
		//Water
		colour = vec3(0., 0., 255.);
	} else if (y < 0.25) {
		//Sand
		colour = vec3(244., 240., 135.);
	} else if(y < 0.5) {
		//Green
		colour = vec3(0., 145., 0.);
	} else if(y < 0.75) {
		colour = vec3(150., 106., 59.);
	} else {
		//WIT, KOUD!!!! :(
		colour = vec3(255., 255., 255.);
	}
	
	fragColour = vec4(colour, 1.);
}