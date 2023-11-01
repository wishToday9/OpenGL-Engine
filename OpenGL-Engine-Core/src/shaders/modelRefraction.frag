#version 450 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube environmentMap;

void main(){
	float refractionIndex = 1.0f / 1.52f;
	vec3 fragToCam = normalize(FragPos - cameraPos);
	vec3 refractionDir = refract(fragToCam, normalize(Normal), refractionIndex);

	color = texture(environmentMap, refractionDir);
}
