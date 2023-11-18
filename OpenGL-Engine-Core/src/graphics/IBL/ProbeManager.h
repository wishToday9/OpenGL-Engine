#pragma once

#include <graphics/Shader.h>
#include <graphics/IBL/LightProbe.h>
#include <graphics/IBL/ReflectionProbe.h>
#include <graphics/Skybox.h>

namespace OpenGL_Engine {
	enum ProbeBlendSetting {
		PROBES_DISABLED,
		PROBES_SIMPLE, //disable blending between probes
		PROBES_BLEND   // Blends adjacent probes
	};

	class ProbeManager {
	public:
		ProbeManager(ProbeBlendSetting sceneProbeBlendingSetting);

		~ProbeManager();

		void init(Skybox* skybox);

		void addProbe(LightProbe* probe);
		void addProbe(ReflectionProbe* probe);

		//Assumes shader is bound
		void bindProbe(glm::vec3& renderPosition, Shader* shader);
	private:
		ProbeBlendSetting m_ProbeBlendSetting;
		std::vector<LightProbe*> m_LightProbes;
		std::vector<ReflectionProbe*> m_ReflectionProbes;

		Skybox* m_Skybox;
	};
}