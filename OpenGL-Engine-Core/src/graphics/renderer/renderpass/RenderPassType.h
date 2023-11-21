#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <platform/OpenGL/Framebuffers/GBuffer.h>

namespace OpenGL_Engine {
	class FrameBuffer;
	enum RenderPassType
	{
		ShadowmapPassType,
		LightingPassType,
		PostProcessPassType,
		ProbePassType,
		GeometryPassType
	};

	struct ShadowmapPassOutput {
		glm::mat4 directionalLightViewProjMatrix;
		FrameBuffer* shadowmapFramebuffer;
	};

	struct LightingPassOutput {
		FrameBuffer* outputFramebuffer;
	};

	struct GeometryPassOutput {
		GBuffer* outputGBuffer;
	};
}
