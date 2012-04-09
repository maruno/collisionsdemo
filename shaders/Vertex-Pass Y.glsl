#version 150 core

in vec3 vertex;
out float y;

void main(void) {
	//Pass vertex as homogenous vertex
	gl_Position = vec4(vertex, 1.);
	
	//Pass Y
	y = vertex.y;
}