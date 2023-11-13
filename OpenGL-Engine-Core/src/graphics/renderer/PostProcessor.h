#pragma once

#include "../../Defs.h"
#include "../mesh/common/Quad.h"
#include "Renderer.h"
#include "../Shader.h"
#include "../../platform/OpenGL/Framebuffers/RenderTarget.h"
#include "../../utils/Timer.h"
#include "../../ui/DebugPane.h"
#include "../../ui/RuntimePane.h"

namespace OpenGL_Engine { namespace graphics {

	class PostProcessor {
	public:
		PostProcessor(Renderer* renderer);
		~PostProcessor();

		void preLightingPostProcess();

		void postLightingPostProcess(opengl::RenderTarget* input);

		void EnableBlur(bool choice);

	private:
		float m_GammaCorrection = 2.2f;

		Renderer* m_Renderer;
		Shader m_PostProcessShader;
		Quad m_NDC_Plane;
		opengl::RenderTarget m_ScreenRenderTarget;
		Timer m_Timer;
		// Post Processing Toggles
		bool m_Blur = false;
	};

} }