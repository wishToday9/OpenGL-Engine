#version 450 core

out vec4 FragColor;

in vec3 SampleDirection;

uniform samplerCube sceneCaptureCubemap;

void main(){
	vec3 normal = normalize(SampleDirection);

	//do the convolution
	vec3 hemisphereIrradiance = texture(sceneCaptureCubemap, normal).rgb;

	FragColor = vec4(hemisphereIrradiance, 1.0);
}