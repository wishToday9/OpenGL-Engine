#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <graphics/Shader.h>
#include <Scene/Scene3D.h>

namespace OpenGL_Engine {
	enum DeferredStencilValue : int {
		ModelStencilValue = 0x01,
		TerrainStencilValue = 0x02
	};

	class DeferredGeometryPass : public RenderPass {
	public:
		DeferredGeometryPass(Scene3D* scene);
		DeferredGeometryPass(Scene3D* scene, GBuffer* customGBuffer);

		virtual ~DeferredGeometryPass();

		GeometryPassOutput executePostLightingPass(ICamera* camera, bool renderOnlyStatic);
	private:
		bool m_AllocatedGBuffer;
		GBuffer* m_GBuffer;
		Shader* m_ModelShader, * m_TerrainShader;

	};
}