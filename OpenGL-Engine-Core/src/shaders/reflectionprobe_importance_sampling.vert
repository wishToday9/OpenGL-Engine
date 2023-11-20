#version 450 core

/*
	This IBL - Reflection Probe Importance Sampling approach is based on the approach Epic made in their paper "Real Shading in Unreal Engine 4"
*/
layout (location = 0) in vec3 position;

out vec3 SampleDirection;

uniform mat4 view;
uniform mat4 projection;

void main(){
	SampleDirection = position;

	//cut off the translation part of the view matrix
	gl_Position = projection * mat4(mat3(view)) * vec4(position, 1.0f);
}