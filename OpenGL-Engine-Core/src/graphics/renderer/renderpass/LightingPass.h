#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <graphics/camera/ICamera.h>


namespace OpenGL_Engine {
	class LightingPass : public RenderPass {
	public:
		LightingPass(Scene3D* scene, bool shouldMultiSample);
		LightingPass(Scene3D* scene, FrameBuffer* customFramebuffer);
		virtual ~LightingPass() override;

		LightingPassOutput executeRenderPass(ShadowmapPassOutput& shadowmapData, ICamera* camera, bool renderOnlyStatic, bool useIBL);
	private:
		void bindShadowmap(Shader* shader, ShadowmapPassOutput& shadowmapData);

		bool m_AllocatedFramebuffer;
		FrameBuffer* m_Framebuffer = nullptr;
		Shader* m_ModelShader, *m_TerrainShader;
	};
}