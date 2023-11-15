#pragma once


#include "ModelRenderer.h"

#include <graphics/Shader.h>
#include <graphics/mesh/common/Quad.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <ui/DebugPane.h>
#include <ui/RuntimePane.h>
#include <utils/Timer.h>

namespace OpenGL_Engine {  

	class PostProcessor {
	public:
		PostProcessor(ModelRenderer* renderer);
		~PostProcessor();

		void preLightingPostProcess();

		void postLightingPostProcess(FrameBuffer* input);

		void EnableBlur(bool choice);

		// Might be useful to have if we want to have more custom post processing. Unity does it this way
		//void blit(Texture *texture, Framebuffer *source);

	private:
		float m_GammaCorrection = 2.2f;

		ModelRenderer* m_ModelRenderer;
		Shader m_PostProcessShader;
		Quad m_NDC_Plane;

		// Only used if multi-sampling is enabled so it can blit to a non-multisampled buffer
		FrameBuffer m_ScreenRenderTarget; 
		Timer m_Timer;
		// Post Processing Toggles
		bool m_Blur = false;
	};

} 