#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace OpenGL_Engine {
	namespace graphics {
		class Material
		{
		public:
			//TODO MOVE to a pbr system
			GLuint diffuseMap, specularMap, emissionMap;
			float shininess;
			GLuint normalMap;
		};
	}
}