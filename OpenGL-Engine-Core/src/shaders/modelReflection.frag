#version 450 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube environmentMap;

void main(){
	vec3 fragToCam = normalize(FragPos - cameraPos);
	vec3 relectionDir = reflect(fragToCam, normalize(Normal));

	color = texture(environmentMap, relectionDir);
}
