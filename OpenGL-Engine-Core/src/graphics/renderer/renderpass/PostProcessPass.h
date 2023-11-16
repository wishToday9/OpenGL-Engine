#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>

namespace OpenGL_Engine {
	class PostProcessPass : public RenderPass {
		  
	public:
		PostProcessPass(Scene3D* scene);
		~PostProcessPass();

		void executeRenderPass(FrameBuffer* framebufferToProcess);

		inline void EnableBlur(bool choice) { m_Blur = choice; }
	private:
		Shader* m_PostProcessShader;

		Quad m_NDC_Plane;
		FrameBuffer m_ScreenRenderTarget; // Only used if multi-sampling is enabled so it can blit to a non-multisampled buffer

		// Post Processing Tweaks
		float m_GammaCorrection = 2.2f;
		bool m_Blur = false;
	};
}