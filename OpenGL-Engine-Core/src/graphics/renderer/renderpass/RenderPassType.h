#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace OpenGL_Engine {
	class FrameBuffer;
	enum RenderPassType
	{
		ShadowmapPassType,
		LightingPassType,
		PostProcessPassType,
		EnvironmentProbePassType
	};

	struct ShadowmapPassOutput {
		glm::mat4 directionalLightViewProjMatrix;
		FrameBuffer* shadowmapFramebuffer;
	};

	struct LightingPassOutput {
		FrameBuffer* outputFramebuffer;
	};
}
