#pragma once

#include <graphics/camera/CubemapCamera.h>
#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/renderer/renderpass/ShadowmapPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>

namespace OpenGL_Engine {
	class EnvironmentProbePass : public RenderPass {
	public:
		EnvironmentProbePass(Scene3D* scene);
		virtual ~EnvironmentProbePass() override;

		void pregenerateProbes();
	private:
		FrameBuffer m_CubemapGenerationFramebuffer;
		CubemapCamera m_CubemapCamera;
	};
}