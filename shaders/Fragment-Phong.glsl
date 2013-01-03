#version 150 core

layout(std140) uniform lightsUni {
	vec4 lightSource[10];
};

layout(std140) uniform cameraUni {
	vec3 viewSource;
};

layout(std140) uniform colourInformationUni {
	vec3 ambiance;
	vec3 kColour;
	vec3 mShininess;
};

in vec3 N;
in vec3 position;

out vec4 fragColour;

void main(void) {
	//Intensity of reflected light
	vec3 Ir = vec3(kColour.r * ambiance.r, kColour.g * ambiance.g, kColour.b * ambiance.b);
	
	//Vector to viewSource
	vec3 E = normalize(viewSource - position);
	
	for(int n = 0; n < 10; n++) {
		//a = Intensity of incident light
		float Ii = lightSource[n].a;
		
		//0.0f intensity means light is off
		if(Ii != 0.0f) {
			//Vector to lightsource
			vec3 L = normalize(lightSource[n].xyz - position);
			
			//Lambert's law: Ir = n dot l
			float IrLambert = dot(N, L);
			
			//Vector bisecting E and L
			vec3 H = normalize(E + L);
			
			//Phong
			float IrPhong = pow(dot(H, N), mShininess.r);
			
			//Light reflection without taking into account the absorbance coefficient
			float IrLuminance = Ii * (IrLambert + IrPhong);
			
			//Red
			Ir.r += kColour.r * IrLuminance;
			
			//Green
			Ir.g += kColour.g * IrLuminance;
			
			//Blue
			Ir.b += kColour.b * IrLuminance;
		}
	}
	
	fragColour = vec4(Ir, 1.);
}
