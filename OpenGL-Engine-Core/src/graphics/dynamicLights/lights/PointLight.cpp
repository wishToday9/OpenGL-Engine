#include "pch.h"
#include "PointLight.h"

namespace OpenGL_Engine {  

	PointLight::PointLight(float intensity, glm::vec3& lightColor, float attenuationRadius, glm::vec3& pos)
		:Light(intensity, lightColor), m_Position(pos), m_AttenuationRadius(attenuationRadius)
	{	}
	// TODO: Assert that the shader is bound in debug
	void PointLight::setupUniforms(Shader *shader, int currentLightIndex) {
		shader->setUniform(("pointLights[" + std::to_string(currentLightIndex) + "].position").c_str(), m_Position);
		shader->setUniform(("pointLights[" + std::to_string(currentLightIndex) + "].intensity").c_str(), m_Intensity);
		shader->setUniform(("pointLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), m_LightColor);
		shader->setUniform(("pointLights[" + std::to_string(currentLightIndex) + "].attenuationRadius").c_str(), m_AttenuationRadius);
	}
} 