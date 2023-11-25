#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out mat3 TBN;
out vec2 TexCoords;

out vec3 FragPosTangentSpace;
out vec3 ViewPosTangentSpace;

uniform bool hasDisplacement;
uniform vec3 viewPos;

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	//use the normal matrix 
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	TBN = mat3(T, B, N);

	TexCoords = texCoords;

	vec3 fragPos = vec3(model * vec4(position, 1.0f));
	if(hasDisplacement){
		mat3 inverseTBN = transpose(TBN); // world space->tangent space
		FragPosTangentSpace = inverseTBN * fragPos;
		ViewPosTangentSpace = inverseTBN * viewPos;
	}

	gl_Position = projection * view * vec4(fragPos, 1.0);
}