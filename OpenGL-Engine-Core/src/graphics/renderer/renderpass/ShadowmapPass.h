#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>
#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <graphics/camera/ICamera.h>

namespace OpenGL_Engine {
	class ShadowmapPass : public RenderPass {
	public:
		ShadowmapPass(Scene3D* scene);
		ShadowmapPass(Scene3D* scene, FrameBuffer* customFramebuffer);
		virtual ~ShadowmapPass() override;

		ShadowmapPassOutput generateShadowmaps(ICamera* camera);
	private:
		FrameBuffer* m_ShadowmapFramebuffer;
		Shader* m_ShadowmapShader;
	};
}