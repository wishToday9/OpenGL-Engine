#include "pch.h"
#include "RenderPass.h"


namespace OpenGL_Engine {
	RenderPass::RenderPass(Scene3D* scene)
		:m_ActiveScene(scene) 
	{
		m_GLCache = GLCache::getInstance();
	}

	RenderPass::~RenderPass() {}
}