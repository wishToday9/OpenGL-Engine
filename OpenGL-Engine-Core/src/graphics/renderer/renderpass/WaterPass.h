#pragma once

#include <graphics/renderer/renderpass/RenderPass.h>
#include <Scene/Scene3D.h>
#include <utils/Timer.h>

namespace OpenGL_Engine {
	class Shader;

	class WaterPass : public RenderPass {
	public:
		WaterPass(Scene3D* scene);
		virtual ~WaterPass() override;

		WaterPassOutput executeWaterPass(ShadowmapPassOutput& shadowmapData, LightingPassOutput& postTransprarency, ICamera* camera);
	private:
		bool m_WaterEnabled;
		Framebuffer m_SceneRefractionFramebuffer, m_SceneReflectionFramebuffer;

#ifdef WATER_REFLECTION_USE_MSAA
		Framebuffer m_ResolveFramebuffer;
#endif

		Shader* m_WaterShader;
		Texture* m_WaveTexture, * m_WaterNormalMap;
		Quad m_WaterPlane;
		glm::vec3 m_WaterPos;
		float m_WaterScale;

		bool m_EnableClearWater, m_EnableShine;
		float m_WaterTiling;
		glm::vec3 m_WaterAlbedo;
		float m_AlbedoPower;
		float m_WaveSpeed;
		float m_WaveMoveFactor;
		float m_WaveStrength;
		float m_ShineDamper;
		float m_WaterNormalSmoothing;
		float m_DepthdampeningEffect;
		float m_ReflectionBias, m_RefractionBias;

		Timer m_Timer;
#if DEBUG_PROFILING
		Timer m_ProfilingTimer;
#endif
	};
}