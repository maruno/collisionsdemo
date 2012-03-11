#version 150 core

in int y;

void main(void) {
	vec3 colour;
	
	if(y < 0) {
		colour = (0, 0, 255);
		//WTF ben je aan het doen? :P
	} else if (y < 20) {
		//Geel
	} else if(y < 40) {
		//Groen
	} else if(y < 75) {
		//Bruin
	} else if(y < 65536) {
		//WIT, KOUD!!!! :(
	}
	
	gl_FragColor = vec4(colour.x, colour.y, colour.z, 1.0);
}