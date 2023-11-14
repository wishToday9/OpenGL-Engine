#pragma once

#include <graphics/textures/Cubemap.h>
#include <graphics/Shader.h>

namespace OpenGL_Engine { namespace graphics {
	class EnvironmentProbe {
	public:
		EnvironmentProbe(glm::vec3& probePosition, glm::vec2& probeResolution);

		void generate();

		//assumes the shader is bound
		void bind(Shader& shader);
	private:
		Cubemap* m_IrradianceMap, * m_PrefilterMap, * m_BRDF_LUT;
		glm::vec3 m_GeneratedPosition;
		bool m_Generated;
	};
} }