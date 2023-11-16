#pragma once

#include <graphics/textures/Cubemap.h>
#include <graphics/Shader.h>

namespace OpenGL_Engine {  
	class EnvironmentProbe {
	public:
		EnvironmentProbe(glm::vec3& probePosition, glm::vec2& probeResolution, bool isStatic);
		~EnvironmentProbe();
		void generate();

		//assumes the shader is bound
		void bind(Shader& shader);

		//get
		inline Cubemap* getIrradianceMap() {
			return m_IrradianceMap;
		}
	private:
		Cubemap* m_IrradianceMap, * m_PrefilterMap, * m_BRDF_LUT;
		glm::vec3 m_Position;
		bool m_Generated;

		glm::vec2 m_ProbeResolution;
		bool m_IsStatic;
	};
} 