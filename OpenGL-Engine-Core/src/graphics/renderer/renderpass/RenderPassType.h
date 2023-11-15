#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace OpenGL_Engine {
	class FrameBuffer;
	enum RenderPassType
	{
		ShadowmapPassType,
		LightingPassType,
		PostProcessPassType
	};

	struct ShadowmapPassOutput {
		glm::mat4 directionalLightViewProjMatrix;
		unsigned int shadowmapTexture;
	};

	struct LightingPassOutput {
		FrameBuffer* outputFramebuffer;
	};
}
