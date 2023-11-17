#pragma once

#include <graphics/Shader.h>
#include <graphics/IBL/EnvironmentProbe.h>
#include <graphics/Skybox.h>

namespace OpenGL_Engine {
	enum EnvironmentProbeBlendSetting {
		PROBES_DISABLED,
		PROBES_SIMPLE, //disable blending between probes
		PROBES_BLEND   // Blends adjacent probes
	};

	class EnvironmentProbeManager {
	public:
		EnvironmentProbeManager(EnvironmentProbeBlendSetting sceneProbeBlendingSetting);

		~EnvironmentProbeManager();

		void init(Skybox* skybox);

		void addProbe(EnvironmentProbe* probe);

		//Assumes shader is bound
		void bindProbe(glm::vec3& renderPosition, Shader* shader);
	private:
		EnvironmentProbeBlendSetting m_ProbeBlendSetting;
		std::vector<EnvironmentProbe*> m_Probes;

		Skybox* m_Skybox;
	};
}