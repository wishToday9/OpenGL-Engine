#pragma once

#include "Light.h"

namespace OpenGL_Engine {  
	class DynamicLightManager;

	class SpotLight : public Light {
		friend DynamicLightManager;
	public:
		SpotLight(glm::vec3 &lightColor, glm::vec3 &pos, glm::vec3 &dir, float cutOffAngle, float outerCutOffAngle);
	
		virtual void setupUniforms(Shader *shader, int currentLightIndex) override;

	private:
		glm::vec3 m_Position, m_Direction;
		float m_CutOff, m_OuterCutOff;
	};

} 