#include "pch.h"

#include "EnvironmentProbeManager.h"

namespace OpenGL_Engine {
	
	EnvironmentProbeManager::EnvironmentProbeManager(EnvironmentProbeBlendSetting sceneProbeBlendingSetting)
		:m_ProbeBlendSetting(sceneProbeBlendingSetting)
	{	}

	EnvironmentProbeManager::~EnvironmentProbeManager()
	{
		for (auto iter = m_Probes.begin(); iter != m_Probes.end(); ++iter) {
			delete (*iter);
		}
		std::vector<EnvironmentProbe*>().swap(m_Probes);
	}

	void EnvironmentProbeManager::init(Skybox* skybox)
	{
		m_Skybox = skybox;
	}

	void EnvironmentProbeManager::addProbe(EnvironmentProbe* probe)
	{
		m_Probes.push_back(probe);
	}

	void EnvironmentProbeManager::bindProbe(glm::vec3& renderPosition, Shader* shader)
	{
		// if simple blending is enabled just use the closest probe
		if (m_ProbeBlendSetting == PROBES_SIMPLE) {
			if (m_Probes.size() > 0) {
				m_Probes[0]->bind(shader);
			}
			else {
				//fall back to skybox
				shader->setUniform1i("irradianceMap", 1);
				m_Skybox->getSkyboxCubemap()->bind(1);
			}
		}
		//if probes are disabled just use the sky box
		else if (m_ProbeBlendSetting == PROBES_DISABLED) {
			//fall back to skybox
			shader->setUniform1i("irradianceMap", 1);
			m_Skybox->getSkyboxCubemap()->bind(1);
		}
		
	}

}