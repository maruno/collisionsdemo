#version 150 core

layout(std140) uniform;

uniform matrixUni {
	mat4 mVPMatrix;
	mat4 mVMatrix;
	mat3 normalMatrix;
};

in vec3 vertex;
in vec3 normal;

out vec3 N;
out vec3 position;

void main(void) {
	//Convert vertex to homogenous
	vec4 hVertex = vec4(vertex, 1.);

	//Pass transformed vertex as homogenous vertex
	gl_Position = mVPMatrix * hVertex;
	
	//Pass transformed normal position as vec3
	vec4 hPosition = mVMatrix * hVertex;
	position = hPosition.xyz / hPosition.w;
	
	//Pass transformed normal
	N = normalize(normalMatrix * normal);
}