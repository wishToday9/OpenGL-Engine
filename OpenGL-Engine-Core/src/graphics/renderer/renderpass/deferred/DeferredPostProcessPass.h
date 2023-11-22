#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <scene/Scene3D.h>

namespace OpenGL_Engine {
	class DeferredPostProcessPass : public RenderPass {
	public:
		DeferredPostProcessPass(Scene3D* scene);
		virtual ~DeferredPostProcessPass() override;

		void executePreLightingPass(Framebuffer* framebufferToProcess);
		void executePostLightingPass(Framebuffer* framebufferToProcess);
		
		void EnableBlur(bool choice) {
			m_Blur = choice;
		}
	private:
		//Down sample
		// two pass blur

	private:

		Shader* m_PostProcessShader;

		//Post Processing tweaks
		float m_GammaCorrection = 2.2f;
		bool m_Blur = false;

	};
}