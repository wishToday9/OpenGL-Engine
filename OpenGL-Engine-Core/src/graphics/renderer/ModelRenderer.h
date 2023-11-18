#pragma once
#include "GLCache.h"

#include <scene/RenderableModel.h>
#include <graphics/camera/FPSCamera.h>
#include <graphics/mesh/Model.h>
#include <graphics/mesh/common/Quad.h>
#include <graphics/mesh/common/Cube.h>
#include <graphics/renderer/renderpass/RenderPassType.h>

namespace OpenGL_Engine {
	class ModelRenderer {
	public:
		ModelRenderer(FPSCamera* camera);

		void submitOpaque(RenderableModel* renderable);
		void submitTransparent(RenderableModel* renderable);

		void flushOpaque(Shader* shader, RenderPassType pass);
		void flushTransparent(Shader* shader, RenderPassType pass);

	public:
		Quad NDC_Plane;
		Cube NDC_Cube;
	private:
		void ModelRenderer::setupModelMatrix(RenderableModel* renderable, Shader* shader, RenderPassType pass);

		std::deque<RenderableModel*> m_OpaqueRenderQueue;
		std::deque<RenderableModel*> m_TransparentRenderQueue;

		// TODO: ADD QUAD TYPE - GOES HERE CALLED m_NDCPLane

		FPSCamera* m_Camera;
		GLCache* m_GLCache;
	};
}