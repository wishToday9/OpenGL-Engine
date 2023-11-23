#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <platform/OpenGL/Framebuffers/GBuffer.h>

namespace OpenGL_Engine {
	class Framebuffer;
	enum RenderPassType
	{
		MaterialRequired,
		NoMaterialRequired
	};

	struct ShadowmapPassOutput {
		glm::mat4 directionalLightViewProjMatrix;
		Framebuffer* shadowmapFramebuffer;
	};

	struct LightingPassOutput {
		Framebuffer* outputFramebuffer;
	};

	struct GeometryPassOutput {
		GBuffer* outputGBuffer;
	};

	struct PreLightingPassOutput {
		Framebuffer* ssaoFramebuffer;
	};
}
