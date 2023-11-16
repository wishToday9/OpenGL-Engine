#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <graphics/camera/ICamera.h>


namespace OpenGL_Engine {
	class LightingPass : RenderPass {
	public:
		LightingPass(Scene3D* scene);
		LightingPass(Scene3D* scene, FrameBuffer* customFramebuffer);
		virtual ~LightingPass() override;

		LightingPassOutput executeRenderPass(ShadowmapPassOutput shadowmapData, ICamera* camera);
	private:
		FrameBuffer* m_Framebuffer = nullptr;
		Shader* m_ModelShader, *m_TerrainShader;

	};
}