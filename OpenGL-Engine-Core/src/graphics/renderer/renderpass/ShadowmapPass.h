#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>

namespace OpenGL_Engine {
	class ShadowmapPass : public RenderPass {
	public:
		ShadowmapPass(Scene3D* scene);
		virtual ~ShadowmapPass() override;

		ShadowmapPassOutput executeRenderPass();
	private:
		FrameBuffer m_ShadowmapFramebuffer;
		Shader m_ShadowmapShader;
	};
}