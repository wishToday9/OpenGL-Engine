#version 450 core
/*
	IBL - BRDF Integration approach is based on the approach Epic made int their paper "Real Shading in	Unreal Engine 4" 
*/
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoords;

void main() {
	gl_Position = vec4(position, 1.0);
	TexCoords = texCoord;
}