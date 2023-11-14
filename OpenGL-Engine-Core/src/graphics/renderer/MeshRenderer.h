#pragma once

#include <deque>
#include <glm\gtx\norm.hpp>

#include "../mesh/Model.h"
#include "../camera/FPSCamera.h"
#include "../../Scene/SceneNode.h"
#include "GLCache.h"
#include "RenderPass.h"
#include "../mesh/common/Quad.h"

namespace OpenGL_Engine {
	namespace graphics {

		class MeshRenderer {
		public:
			MeshRenderer(FPSCamera* camera);

			void submitOpaque(scene::SceneNode* renderable);
			void submitTransparent(scene::SceneNode* renderable);

			void flushOpaque(Shader& shader, RenderPass pass);
			void flushTransparent(Shader& shader, RenderPass pass);

		public:
			Quad NDC_Plane;
		private:
			void MeshRenderer::setupModelMatrix(scene::SceneNode* renderable, Shader& shader, RenderPass pass);

			std::deque<scene::SceneNode*> m_OpaqueRenderQueue;
			std::deque<scene::SceneNode*> m_TransparentRenderQueue;

			// TODO: ADD QUAD TYPE - GOES HERE CALLED m_NDCPLane

			FPSCamera* m_Camera;
			GLCache* m_GLCache;
		};
	}
}