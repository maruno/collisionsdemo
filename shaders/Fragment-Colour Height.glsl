#version 150 core

in float y;

void main(void) {
	vec3 colour;
	
	if(y < 0.0) {
		//Water
		colour = (0, 0, 255);
	} else if (y < 0.25) {
		//Sand
		colour = (244, 240, 135);
	} else if(y < 0.5) {
		//Green
		colour = (0, 145, 0);
	} else if(y < 0.75) {
		colour = (150, 106, 59);
	} else {
		//WIT, KOUD!!!! :(
		colour = (255, 255, 255);
	}
	
	gl_FragColor = vec4(colour, 1.0);
}