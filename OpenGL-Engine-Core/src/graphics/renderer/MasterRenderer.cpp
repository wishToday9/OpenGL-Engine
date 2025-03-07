#include "pch.h"
#include "MasterRenderer.h"

#include <ui/RuntimePane.h>


namespace OpenGL_Engine
{

	MasterRenderer::MasterRenderer(Scene3D* scene) : m_ActiveScene(scene),
		m_ShadowmapPass(scene), m_PostProcessPass(scene), m_ForwardLightingPass(scene, true),
		m_EnvironmentProbePass(scene), m_DeferredGeometryPass(scene), m_DeferredLightingPass(scene),
		m_PostGBufferForwardPass(scene), m_WaterPass(m_ActiveScene)
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
#if FORWARD_RENDER
		// Shadow map pass
#if DEBUG_PROFILING
		glFinish();
		m_ProfilingTimer.reset();
#endif
		ShadowmapPassOutput shadowmapOutput = m_ShadowmapPass.generateShadowmaps(m_ActiveScene->getCamera(), false);
#if DEBUG_PROFILING
		glFinish();
		RuntimePane::setShadowmapTimer((float)m_ProfilingTimer.elapsed());
#endif
		LightingPassOutput lightingOutput = m_ForwardLightingPass.executeLightingPass(shadowmapOutput, m_ActiveScene->getCamera(), false, true);
		WaterPassOutput waterOutput = m_WaterPass.executeWaterPass(lightingOutput, m_ActiveScene->getCamera());
		m_PostProcessPass.executePostProcessPass(waterOutput.outputFramebuffer);


		/* Deferred Rendering */
#else
#if DEBUG_PROFILING
		glFinish();
		m_ProfilingTimer.reset();
#endif
		ShadowmapPassOutput shadowmapOutput = m_ShadowmapPass.generateShadowmaps(m_ActiveScene->getCamera(), false);
#if DEBUG_PROFILING
		glFinish();
		RuntimePane::setShadowmapTimer((float)m_ProfilingTimer.elapsed());
#endif


		GeometryPassOutput geometryOutput = m_DeferredGeometryPass.executeGeometryPass(m_ActiveScene->getCamera(), false);
		PreLightingPassOutput preLightingOutput = m_PostProcessPass.executePreLightingPass(geometryOutput, m_ActiveScene->getCamera());
		LightingPassOutput deferredLightingOutput = m_DeferredLightingPass.executeLightingPass(shadowmapOutput, geometryOutput, preLightingOutput, m_ActiveScene->getCamera(), true);
		LightingPassOutput postGBufferForward = m_PostGBufferForwardPass.executeLightingPass(shadowmapOutput, deferredLightingOutput, m_ActiveScene->getCamera(), false, true);
		WaterPassOutput waterOutput = m_WaterPass.executeWaterPass(shadowmapOutput, postGBufferForward, m_ActiveScene->getCamera());
		m_PostProcessPass.executePostProcessPass(waterOutput.outputFramebuffer);
#endif
	}

}