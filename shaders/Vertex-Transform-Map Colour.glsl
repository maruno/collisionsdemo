#version 150 core

layout(std140) uniform;

uniform hmMatrices {
	mat4 mVPMatrix;
};

uniform hmHeight {
	uint maxHeight;
};

in vec3 vertex;
out vec3 colour;

void main(void) {
	//Pass transformed vertex as homogenous vertex
	gl_Position = mVPMatrix * vec4(vertex, 1.);
	
	float height = vertex.y / maxHeight;
	
	//Basic map colours
	vec3 black = vec3(0., 0., 0.);
	vec3 blue = vec3(0., 0., 255./255.);
	vec3 sand = vec3(244./255., 240./255., 135./255.);
	vec3 green = vec3(0., 145./255., 0.);
	vec3 brown = vec3(150./255., 106./255., 59./255.);
	vec3 white = vec3(255./255., 255./255., 255./255.);
	
	//Determine smooth colour transition for map
	if(height < 0.0) {
		//Water
		float a = smoothstep(-0.25, 0.0, height);
		colour = mix(black, blue, a);
	} else if (height < 0.25) {
		//Sand
		float a = smoothstep(0.0, 0.25, height);
		colour = mix(sand, green, a);
	} else if(height < 0.5) {
		//Green
		float a = smoothstep(0.25, 0.5, height);
		colour = mix(green, brown, a);
	} else {
		float a = smoothstep(0.5, 1.0, height);
		colour = mix(brown, white, a);
	}
}