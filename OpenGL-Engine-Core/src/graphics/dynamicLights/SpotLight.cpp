#include "pch.h"
#include "SpotLight.h"
namespace OpenGL_Engine {  

	SpotLight::SpotLight(glm::vec3& lightColor, glm::vec3& pos, glm::vec3& dir, float cutOffAngle, float outerCutOffAngle)
		: DynamicLight(lightColor), position(pos), direction(dir), cutOff(cutOffAngle), outerCutOff(outerCutOffAngle)
	{}

	// TODO: Add in multiple spotlight support
	// TODO: Assert that the shader is bound in debug
	void SpotLight::setupUniforms(Shader *shader, int currentLightIndex) {
		if (isActive) {
			shader->setUniform3f("spotLight.position", position);
			shader->setUniform3f("spotLight.direction", direction);
			shader->setUniform3f("spotLight.lightColor", lightColor);
			shader->setUniform1f("spotLight.cutOff", cutOff);
			shader->setUniform1f("spotLight.outerCutOff", outerCutOff);
		}
	}

} 