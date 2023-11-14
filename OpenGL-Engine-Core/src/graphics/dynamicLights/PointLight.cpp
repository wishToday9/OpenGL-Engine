
#include "PointLight.h"

namespace OpenGL_Engine { namespace graphics {


	PointLight::PointLight(glm::vec3& lightColor, glm::vec3& pos)
		:DynamicLight(lightColor), position(pos)
	{

	}

	// TODO: Assert that the shader is bound in debug
	void PointLight::setupUniforms(Shader &shader, int currentLightIndex) {
		if (isActive) {
			shader.setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].position").c_str(), position);
			shader.setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), lightColor);
		}
	}

} }