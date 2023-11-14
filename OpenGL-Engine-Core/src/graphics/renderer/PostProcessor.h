#pragma once


#include "MeshRenderer.h"

#include <graphics/Shader.h>
#include <graphics/mesh/common/Quad.h>
#include <platform/OpenGL/Framebuffers/RenderTarget.h>
#include <ui/DebugPane.h>
#include <ui/RuntimePane.h>
#include <utils/Timer.h>

namespace OpenGL_Engine { namespace graphics {

	class PostProcessor {
	public:
		PostProcessor(MeshRenderer* renderer);
		~PostProcessor();

		void preLightingPostProcess();

		void postLightingPostProcess(opengl::RenderTarget* input);

		void EnableBlur(bool choice);

	private:
		float m_GammaCorrection = 2.2f;

		MeshRenderer* m_MeshRenderer;
		Shader m_PostProcessShader;
		Quad m_NDC_Plane;
		opengl::RenderTarget m_ScreenRenderTarget;
		Timer m_Timer;
		// Post Processing Toggles
		bool m_Blur = false;
	};

} }