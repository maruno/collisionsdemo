#version 150 core

layout(std140) uniform;

uniform hmMatrices {
	mat4 mVPMatrix;
};

in vec3 vertex;
out float y;

void main(void) {
	//Pass vertex as homogenous vertex
	gl_Position = mVPMatrix * vec4(vertex, 1.);
	
	//Pass Y
	y = vertex.y;
}