#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <graphics/camera/ICamera.h>


namespace OpenGL_Engine {
	class ForwardLightingPass : public RenderPass {
	public:
		ForwardLightingPass(Scene3D* scene, bool shouldMultiSample);
		ForwardLightingPass(Scene3D* scene, FrameBuffer* customFramebuffer);
		virtual ~ForwardLightingPass() override;

		LightingPassOutput executeRenderPass(ShadowmapPassOutput& shadowmapData, ICamera* camera, bool renderOnlyStatic, bool useIBL);
	private:
		void bindShadowmap(Shader* shader, ShadowmapPassOutput& shadowmapData);

		bool m_AllocatedFramebuffer;
		FrameBuffer* m_Framebuffer = nullptr;
		Shader* m_ModelShader, *m_TerrainShader;
	};
}