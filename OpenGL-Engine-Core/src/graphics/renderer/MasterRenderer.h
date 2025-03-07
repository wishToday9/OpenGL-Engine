#pragma once
#include <graphics/renderer/renderpass/deferred/DeferredGeometryPass.h>
#include <graphics/renderer/renderpass/deferred/DeferredLightingPass.h>
#include <graphics/renderer/renderpass/deferred/PostGBufferForward.h>
#include <graphics/renderer/renderpass/forward/ForwardLightingPass.h>
#include <graphics/renderer/renderpass/forward/ForwardProbePass.h>
#include <graphics/renderer/renderpass/PostProcessPass.h>
#include <graphics/renderer/renderpass/ShadowmapPass.h>
#include <graphics/renderer/renderpass/WaterPass.h>

#include <scene/Scene3D.h>
#include <utils/Timer.h>

namespace OpenGL_Engine {
	class MasterRenderer {
	public:
		MasterRenderer(Scene3D* scene);

		void init();
		void render();
	private:
		GLCache* m_GLCache;
		Scene3D* m_ActiveScene;

		// Other passes
		ShadowmapPass m_ShadowmapPass;
		PostProcessPass m_PostProcessPass;

		// Forward passes
		ForwardLightingPass m_ForwardLightingPass;
		ForwardProbePass m_EnvironmentProbePass;

		// Deferred passes
		DeferredGeometryPass m_DeferredGeometryPass;
		DeferredLightingPass m_DeferredLightingPass;
		PostGBufferForward m_PostGBufferForwardPass;
		WaterPass m_WaterPass;

		Timer m_ProfilingTimer;
	};
}