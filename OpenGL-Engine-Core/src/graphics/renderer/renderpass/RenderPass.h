#pragma once

#include "RenderPassType.h"

#include <Scene/Scene3D.h>

namespace OpenGL_Engine {
	class RenderPass {
	public:
		RenderPass(Scene3D* scene);
		virtual ~RenderPass();
	protected:
		GLCache* m_GLCache;
		Scene3D* m_ActiveScene;
	};
}