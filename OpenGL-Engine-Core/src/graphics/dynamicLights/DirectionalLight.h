#pragma once

#include "DynamicLight.h"

namespace OpenGL_Engine { namespace graphics {

	struct DirectionalLight : public DynamicLight {
		DirectionalLight();

		DirectionalLight(glm::vec3 &lightColor, glm::vec3 &dir);

		virtual void setupUniforms(Shader &shader, int currentLightIndex) override;


		glm::vec3 direction;
	};

} }