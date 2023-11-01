
#pragma once

#include "DynamicLight.h"

namespace OpenGL_Engine {
	namespace graphics {

		class SpotLight : public DynamicLight {
		public:
			SpotLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, 
				glm::vec3& position, glm::vec3& direction, float cutOff, float outerCutOff, 
				float constant, float linear, float quadratic);

			virtual void setupUniforms(Shader& shader, int currentLightIndex) override;

			glm::vec3 position, direction;
			float cutOff, outerCutOff;
			float constant, linear, quadratic;
		};

	}
}