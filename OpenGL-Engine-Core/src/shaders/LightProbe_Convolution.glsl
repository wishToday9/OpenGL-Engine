#shader-type vertex
#version 450 core
layout(location = 0) in vec3 position;

out vec3 SampleDirection;
uniform mat4 view;
uniform mat4 projection;

void main() {
	SampleDirection = position;

	gl_Position = projection * mat4(mat3(view)) * vec4(position, 1.0f);
}

#shader-type fragment
#version 450 core

const float PI = 3.14159265359;

out vec4 FragColor;

in vec3 SampleDirection;

uniform samplerCube sceneCaptureCubemap;

void main(){
	vec3 normal = normalize(SampleDirection);

	//we need the axis to convert out vector form tangent to world space
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up         = normalize(cross(normal, right));

	//search the hemisphere and sample for the convolution
	vec3 hemisphereIrradiance = vec3(0.0, 0.0, 0.0);
	float numSamples = 0;


	//²Î¿¼¹«Ê½: https://uploads.disquscdn.com/images/db998524cbb47d852b788e2ea8b6d4f1077cdc9e92fc38ea3c30fde37d2e32c2.png
	float delta = 0.025; //0.05 results in 126 * 32 = 4032 samples
	for(float phi = 0.0; phi < PI * 2.0; phi += delta){
		for(float theta = 0.0; theta < PI * 0.5; theta += delta){
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world space
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 
			// Scaled by sin(theta) to account for smaller sample areas in the higher hemipshere rings, and scaled by cos(theta) due to light being weaker at larger angles
            hemisphereIrradiance += texture(sceneCaptureCubemap, sampleVec).rgb * cos(theta) * sin(theta);
            numSamples++;
		}
	}
	vec3 irradiance = PI * hemisphereIrradiance * (1.0 / numSamples);

	FragColor = vec4(irradiance, 1.0);
}