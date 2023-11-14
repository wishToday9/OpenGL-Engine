#pragma once


#include "MeshRenderer.h"

#include <graphics/Shader.h>
#include <graphics/mesh/common/Quad.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <ui/DebugPane.h>
#include <ui/RuntimePane.h>
#include <utils/Timer.h>

namespace OpenGL_Engine {  

	class PostProcessor {
	public:
		PostProcessor(MeshRenderer* renderer);
		~PostProcessor();

		void preLightingPostProcess();

		void postLightingPostProcess(FrameBuffer* input);

		void EnableBlur(bool choice);

		// Might be useful to have if we want to have more custom post processing. Unity does it this way
		//void blit(Texture *texture, Framebuffer *source);

	private:
		float m_GammaCorrection = 2.2f;

		MeshRenderer* m_MeshRenderer;
		Shader m_PostProcessShader;
		Quad m_NDC_Plane;
		FrameBuffer m_ScreenRenderTarget;
		Timer m_Timer;
		// Post Processing Toggles
		bool m_Blur = false;
	};

} 