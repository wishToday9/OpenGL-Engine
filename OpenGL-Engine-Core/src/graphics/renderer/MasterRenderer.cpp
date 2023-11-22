#include "pch.h"
#include "MasterRenderer.h"

#include <ui/RuntimePane.h>


namespace OpenGL_Engine
{

	MasterRenderer::MasterRenderer(Scene3D* scene) : m_ActiveScene(scene),
		m_ShadowmapPass(scene), m_ForwardLightingPass(scene, true), m_ForwardPostProcessPass(scene), m_EnvironmentProbePass(scene),
		m_DeferredGeometryPass(scene), m_DeferredLightingPass(scene), m_DeferredPostProcessPass(scene)
	
	{
		m_GLCache = GLCache::getInstance();
	}


	void MasterRenderer::init()
	{
		//state that should never change
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		m_EnvironmentProbePass.pregenerateIBL();
		m_EnvironmentProbePass.pregenerateProbes();
	}

	void MasterRenderer::render() {
//		// Shadow map pass
//#if DEBUG_ENABLED
//		glFinish();
//		m_Timer.reset();
//#endif
//		ShadowmapPassOutput shadowmapOutput = m_ShadowmapPass.generateShadowmaps(m_ActiveScene->getCamera(), false);
//#if DEBUG_ENABLED
//		glFinish();
//		RuntimePane::setShadowmapTimer((float)m_Timer.elapsed());
//#endif
//		// Lighting Pass
//		LightingPassOutput lightingOutput = m_ForwardLightingPass.executePostLightingPass(shadowmapOutput, m_ActiveScene->getCamera(), false,true);
//
//		// Post Process Pass
//#if DEBUG_ENABLED
//		glFinish();
//		m_Timer.reset();
//#endif
//		m_ForwardPostProcessPass.executePostLightingPass(lightingOutput.outputFramebuffer);
//#if DEBUG_ENABLED
//		glFinish();
//		RuntimePane::setPostProcessTimer((float)m_Timer.elapsed());
//#endif

		
		//deferred testing
		ShadowmapPassOutput defshadowmapOutput = m_ShadowmapPass.generateShadowmaps(m_ActiveScene->getCamera(), false);
		GeometryPassOutput geometryOutput = m_DeferredGeometryPass.executePostLightingPass(m_ActiveScene->getCamera(), false);
		LightingPassOutput deferredLightingOutput = m_DeferredLightingPass.executePostLightingPass(defshadowmapOutput, geometryOutput, m_ActiveScene->getCamera(), true);
		m_DeferredPostProcessPass.executePostLightingPass(deferredLightingOutput.outputFramebuffer);
		
	}

}