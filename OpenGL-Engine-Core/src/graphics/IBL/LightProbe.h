#pragma once

#include <graphics/textures/Cubemap.h>
#include <graphics/Shader.h>

namespace OpenGL_Engine {  
	class LightProbe {
	public:
		LightProbe(glm::vec3& probePosition, glm::vec2& probeResolution);
		~LightProbe();
		void generate();

		//assumes the shader is bound
		void bind(Shader* shader);

		//get
		inline Cubemap* getIrradianceMap() {
			return m_IrradianceMap;
		}
	private:
		Cubemap* m_IrradianceMap;

		glm::vec3 m_Position;
		glm::vec2 m_ProbeResolution;
		bool m_Generated;
	};
} 