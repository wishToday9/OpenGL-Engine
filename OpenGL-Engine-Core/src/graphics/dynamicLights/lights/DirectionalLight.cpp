#include "pch.h"
#include "DirectionalLight.h"

namespace OpenGL_Engine {  

	DirectionalLight::DirectionalLight(glm::vec3& lightColour, glm::vec3& dir)
		: Light(lightColour), m_Direction(dir) {}

	// TODO: Add in multiple directional light support
	// TODO: Assert that the shader is bound in debug
	void DirectionalLight::setupUniforms(Shader* shader, int currentLightIndex) {
		shader->setUniform3f(("dirLights[" + std::to_string(currentLightIndex) + "].direction").c_str(), m_Direction);
		shader->setUniform3f(("dirLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), m_LightColor);
	}
} 
