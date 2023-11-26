#include "pch.h"
#include "SpotLight.h"
namespace OpenGL_Engine {  



	SpotLight::SpotLight(float intensity, glm::vec3& lightColor, float attenuationRadius, glm::vec3& pos, glm::vec3& dir, float cutOffAngle, float outerCutOffAngle)
		: Light(intensity, lightColor), m_AttenuationRadius(attenuationRadius), m_Position(pos),
		m_Direction(dir), m_CutOff(cutOffAngle), m_OuterCutOff(outerCutOffAngle)
	{

	}

	// TODO: Add in multiple spotlight support
	// TODO: Assert that the shader is bound in debug
	void SpotLight::setupUniforms(Shader *shader, int currentLightIndex) {
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].position").c_str(), m_Position);
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].direction").c_str(), m_Direction);
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].intensity").c_str(), m_Intensity);
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), m_LightColor);
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].attenuationRadius").c_str(), m_AttenuationRadius);
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].cutOff").c_str(), m_CutOff);
		shader->setUniform(("spotLights[" + std::to_string(currentLightIndex) + "].outerCutOff").c_str(), m_OuterCutOff);
	}
} 