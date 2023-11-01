#include "PointLight.h"
namespace OpenGL_Engine {
	namespace graphics {


		PointLight::PointLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic)
			:DynamicLight(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic)
		{

		}

		void PointLight::setupUniforms(Shader& shader, int currentLightIndex)
		{
			shader.setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].ambient").c_str(), ambient);
			shader.setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].diffuse").c_str(), diffuse);
			shader.setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].specular").c_str(), specular);
			shader.setUniform3f(("pointLights[" + std::to_string(currentLightIndex) + "].position").c_str(), position);
			shader.setUniform1f(("pointLights[" + std::to_string(currentLightIndex) + "].constant").c_str(), constant);
			shader.setUniform1f(("pointLights[" + std::to_string(currentLightIndex) + "].linear").c_str(), linear);
			shader.setUniform1f(("pointLights[" + std::to_string(currentLightIndex) + "].quadratic").c_str(), quadratic);
		}

	}
}