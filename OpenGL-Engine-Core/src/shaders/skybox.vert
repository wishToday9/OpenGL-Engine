#version 450 core

layout(location = 0) in vec3 position;

out vec3 SampleDirection;

uniform mat4 view;
uniform mat4 projection;

void main(){
	
	gl_Position = projection * mat4(mat3(view)) * vec4(position, 1.0f);
	SampleDirection = position;
}