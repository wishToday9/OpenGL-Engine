#include "DirectionalLight.h"
namespace OpenGL_Engine {
	namespace graphics {
		
		DirectionalLight::DirectionalLight(glm::vec3& amb, glm::vec3& dif, glm::vec3& spec, glm::vec3& dir)
			:DynamicLight(amb, dif, spec), direction(dir)
		{
		}

		//to do add in mutiple directional light support
		void DirectionalLight::setupUniforms(Shader& shader, int currentLightIndex)
		{
			shader.setUniform3f("dirLight.ambient", ambient);
			shader.setUniform3f("dirLight.diffuse", diffuse);
			shader.setUniform3f("dirLight.specular", specular);
			shader.setUniform3f("dirLight.direction", direction);
		}

	}
}