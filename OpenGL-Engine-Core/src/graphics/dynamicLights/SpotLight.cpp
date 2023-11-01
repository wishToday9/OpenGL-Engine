#include "SpotLight.h"
namespace OpenGL_Engine {
	namespace graphics {

		SpotLight::SpotLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, 
			glm::vec3& position, glm::vec3& direction, float cutOff, float outerCutOff, 
			float constant, float linear, float quadratic)
			: DynamicLight(ambient, diffuse, specular), position(position), direction(direction),
			cutOff(cutOff), outerCutOff(outerCutOff), constant(constant), linear(linear), quadratic(quadratic) {}


		void SpotLight::setupUniforms(Shader& shader, int currentLightIndex)
		{
			shader.setUniform3f("spotLight.ambient", ambient);
			shader.setUniform3f("spotLight.diffuse", diffuse);
			shader.setUniform3f("spotLight.specular", specular);
			shader.setUniform3f("spotLight.position", position);
			shader.setUniform3f("spotLight.direction", direction);
			shader.setUniform1f("spotLight.cutOff", cutOff);
			shader.setUniform1f("spotLight.outerCutOff", outerCutOff);
			shader.setUniform1f("spotLight.constant", constant);
			shader.setUniform1f("spotLight.linear", linear);
			shader.setUniform1f("spotLight.quadratic", quadratic);
		}

	}
}