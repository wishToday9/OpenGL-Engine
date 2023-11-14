#include "EnvironmentProbeManager.h"
namespace OpenGL_Engine {
	
	EnvironmentProbeManager::EnvironmentProbeManager()
		:m_CubemapGenerationFramebuffer(DEFAULT_IBL_RESOLUTION, DEFAULT_IBL_RESOLUTION)
	{

	}

	void EnvironmentProbeManager::generateProbe(glm::vec3& worldPosition, glm::vec2& probeResolution, bool isStaticProbe)
	{
		EnvironmentProbe* iblProbe = new EnvironmentProbe(worldPosition, probeResolution, isStaticProbe);

		m_CubemapGenerationFramebuffer = FrameBuffer((unsigned int)probeResolution.x, (unsigned int)probeResolution.y);
		m_CubemapGenerationFramebuffer.addDepthStencilRBO(false);
	}
}