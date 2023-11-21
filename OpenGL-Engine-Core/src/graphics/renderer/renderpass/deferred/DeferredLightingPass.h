#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>

namespace OpenGL_Engine {
	class DeferredLightingPass : public RenderPass {
	public:
		DeferredLightingPass(Scene3D* scene);
		DeferredLightingPass(Scene3D* scene, FrameBuffer* framebuffer);

		virtual ~DeferredLightingPass() override;

		LightingPassOutput executeRenderPass(ShadowmapPassOutput& shadowmapData,
			GeometryPassOutput& geometryData, ICamera* camera, bool useIBL);
	private:
		void bindShadowmap(Shader* shader, ShadowmapPassOutput& shadowmapData);

	private:
		bool m_AllocatedFramebuffer;
		FrameBuffer* m_Framebuffer;
		Shader* m_LightingShader;
	};

}