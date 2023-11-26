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

		WaterPassOutput executeWaterPass(LightingPassOutput& postTransprarency, ICamera* camera);
	private:
		bool m_WaterEnabled;
		Framebuffer m_SceneShadowFramebuffer, m_SceneRefractionFramebuffer, m_SceneReflectionFramebuffer;
		
		Shader* m_WaterShader;
		Texture* m_WaveTexture, *m_WaterNormalMap;

		Quad m_WaterPlane;
		glm::vec3 m_WaterPos;
		float m_WaterScale;

		bool m_EnableClearWater;
		glm::vec3 m_WaterAlbedo;
		float m_AlbedoPower;
		float m_WaveSpeed;
		float m_WaveMoveFactor;

		Timer m_Timer;
	};
}