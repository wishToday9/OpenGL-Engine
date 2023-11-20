#include "pch.h"
#include "PointLight.h"

namespace OpenGL_Engine {  

	PointLight::PointLight(glm::vec3& lightColor, glm::vec3& pos)
		:Light(lightColor), m_Position(pos)
	{	}
	// TODO: Assert that the shader is bound in debug
	void PointLight::setupUniforms(Shader *shader, int currentLightIndex) {
		shader->setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].position").c_str(), m_Position);
		shader->setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), m_LightColor);
	}
} 