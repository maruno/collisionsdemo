#version 150 core

layout(std140) uniform;

uniform mVPMatrixUni {
	mat4 mVPMatrix;
};

in vec3 vertex;

void main(void) {
	//Pass transformed vertex as homogenous vertex
	gl_Position = mVPMatrix * vec4(vertex, 1.);
}