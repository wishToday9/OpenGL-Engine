#pragma once

#include "DynamicLight.h"

namespace OpenGL_Engine {
	namespace graphics {
	
		class DirectionalLight : public DynamicLight
		{
		public:
			DirectionalLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& direction);

			virtual void setupUniforms(Shader& shader, int currentLightIndex) override;

			glm::vec3 direction;
		};
	
	}
}

