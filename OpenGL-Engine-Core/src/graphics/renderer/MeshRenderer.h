#pragma once

#include <deque>
#include <glm\gtx\norm.hpp>

#include "../mesh/Model.h"
#include "../camera/FPSCamera.h"
#include "../../Scene/RenderableModel.h"
#include "GLCache.h"
#include "RenderPass.h"
#include "../mesh/common/Quad.h"

namespace OpenGL_Engine {

	class MeshRenderer {
	public:
		MeshRenderer(FPSCamera* camera);

		void submitOpaque(RenderableModel* renderable);
		void submitTransparent(RenderableModel* renderable);

		void flushOpaque(Shader& shader, RenderPass pass);
		void flushTransparent(Shader& shader, RenderPass pass);

	public:
		Quad NDC_Plane;
	private:
		void MeshRenderer::setupModelMatrix(RenderableModel* renderable, Shader& shader, RenderPass pass);

		std::deque<RenderableModel*> m_OpaqueRenderQueue;
		std::deque<RenderableModel*> m_TransparentRenderQueue;

		// TODO: ADD QUAD TYPE - GOES HERE CALLED m_NDCPLane

		FPSCamera* m_Camera;
		GLCache* m_GLCache;
	};

}