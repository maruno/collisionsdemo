#version 150 core

in vec3 vertex;
out int y;

void main(void) {
	//Pass vertex as homogenous vertex
	gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1);
	
	//Pass Y
	y = vertex.y;
}