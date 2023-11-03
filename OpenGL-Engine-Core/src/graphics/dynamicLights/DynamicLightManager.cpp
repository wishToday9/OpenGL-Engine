#include "DynamicLightManager.h"

namespace OpenGL_Engine {
	namespace graphics {
		
		// add functionality so it can update with an entity's position and orientation
		DynamicLightManager::DynamicLightManager()
			: m_DirectionalLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
			m_SpotLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
		{
			init();
		}

		void DynamicLightManager::setupLightingUniforms(Shader& shader)
		{
			shader.setUniform1i("numPointLights", m_PointLights.size());

			if (m_DirectionalLight.isActive) {
				m_DirectionalLight.setupUniforms(shader, 0);
			}
			if (m_SpotLight.isActive) {
				m_SpotLight.setupUniforms(shader, 0);
			}
			for (int i = 0; i < m_PointLights.size(); ++i) {
				if (m_PointLights[i].isActive) {
					m_PointLights[i].setupUniforms(shader, i);
				}
			}
		}

		void DynamicLightManager::addPointLight(PointLight& pointLight)
		{
			m_PointLights.push_back(pointLight);
		}

		void DynamicLightManager::init()
		{		
			// Setup lighting configurations
			m_DirectionalLight.isActive = true;
			m_DirectionalLight.direction = glm::vec3(-0.3f, -1.0f, -0.3f);
			m_DirectionalLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
			m_DirectionalLight.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
			m_DirectionalLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

			m_SpotLight.isActive = true;
			m_SpotLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
			m_SpotLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			m_SpotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
			m_SpotLight.position = glm::vec3(0.0f, 0.0f, 0.0f);
			m_SpotLight.direction = glm::vec3(1.0f, 0.0f, 0.0f);
			m_SpotLight.constant = 1.0f;
			m_SpotLight.linear = 0.022f;
			m_SpotLight.quadratic = 0.0019f;
			m_SpotLight.cutOff = glm::cos(glm::radians(12.5f));
			m_SpotLight.outerCutOff = glm::cos(glm::radians(15.0f));

			PointLight pointLight(glm::vec3(0.05f, 0.05f, 0.05f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(30.0f, -10.0f, 30.0f),
				1.0f,
				0.007f,
				0.0002f);
			pointLight.isActive = true;
			addPointLight(pointLight);

		}

	}
}