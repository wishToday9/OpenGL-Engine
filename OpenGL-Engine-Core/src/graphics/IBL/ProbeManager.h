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


		inline void setLightProbeFallback(LightProbe* probe) { m_LightProbeFallback = probe; }
		inline void setReflectionProbeFallback(ReflectionProbe* probe) { m_ReflectionProbeFallback = probe; }

		//Assumes shader is bound
		void bindProbes(glm::vec3& renderPosition, Shader* shader);
	private:
		ProbeBlendSetting m_ProbeBlendSetting;

		//scene probes
		std::vector<LightProbe*> m_LightProbes;
		std::vector<ReflectionProbe*> m_ReflectionProbes;

		//fall back probes
		LightProbe* m_LightProbeFallback;
		ReflectionProbe* m_ReflectionProbeFallback;

		Skybox* m_Skybox;
	};
}