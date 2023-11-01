#pragma once

#include <glm/common.hpp>

#include "../Shader.h"

namespace OpenGL_Engine {
	namespace graphics {

		struct DynamicLight
		{
			DynamicLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular);
			virtual void setupUniforms(Shader& shader, int currentLightIndex) = 0;
			glm::vec3 ambient, diffuse, specular;
			bool isActive;
		};
	
	}
}

