#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/Framebuffer.h>

namespace OpenGL_Engine {
	class PostProcessPass : public RenderPass {
		  
	public:
		PostProcessPass(Scene3D* scene);
		~PostProcessPass();

		void executePostProcessPass(Framebuffer* framebufferToProcess);
	private:
		Shader* m_PostProcessShader;
		Shader* m_FxaaShader;

		Framebuffer m_TonemappedNonLinearTarget;
		Framebuffer m_ScreenRenderTarget; // Only used if the render resolution differs from the window resolution
		Framebuffer m_ResolveRenderTarget; // Only used if multi-sampling is enabled so it can be resolved
		// Post Processing Tweaks
		float m_GammaCorrection = 2.2f;
	};
}