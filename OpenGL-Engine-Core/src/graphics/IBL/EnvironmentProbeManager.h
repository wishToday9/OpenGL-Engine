#pragma once

#include <graphics/camera/CubemapCamera.h>
#include <graphics/IBL/EnvironmentProbe.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>

namespace OpenGL_Engine {
	class EnvironmentProbeManager {
	public:
		EnvironmentProbeManager();

		void generateProbe(glm::vec3& worldPosition, glm::vec2& probeResolution, bool isStaticProbe);

	private:
		FrameBuffer m_CubemapGenerationFramebuffer;
		CubemapCamera m_CubemapCamera;
		std::vector<EnvironmentProbe> m_EnvironmentProbes;
	};
}