#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>

namespace OpenGL_Engine {
	class LightingPass : RenderPass {
	public:
		LightingPass(Scene3D* scene);
		virtual ~LightingPass() override;

		LightingPassOutput executeRenderPass(ShadowmapPassOutput shadowmapData);
	private:
		FrameBuffer m_Framebuffer;
		Shader m_ModelShader, m_TerrainShader;

	};
}