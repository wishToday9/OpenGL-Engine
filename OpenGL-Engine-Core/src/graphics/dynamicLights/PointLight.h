#pragma once

#include "DynamicLight.h"

namespace OpenGL_Engine {  

	struct PointLight : public DynamicLight {
	public:
		PointLight(glm::vec3& lightColor, glm::vec3 &pos);

		virtual void setupUniforms(Shader *shader, int currentLightIndex) override;


		glm::vec3 position;
	};

} 