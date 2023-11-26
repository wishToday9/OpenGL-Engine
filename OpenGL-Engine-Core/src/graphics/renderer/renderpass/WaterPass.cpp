#include "pch.h"
#include "WaterPass.h"

#include <graphics/Shader.h>
#include <utils/loaders/ShaderLoader.h>
#include <graphics/renderer/renderpass/forward/ForwardLightingPass.h>
#include <graphics/renderer/renderpass/ShadowmapPass.h>

namespace OpenGL_Engine {
	
	WaterPass::WaterPass(Scene3D* scene) : RenderPass(scene), m_WaterEnabled(true), 
		m_SceneShadowFramebuffer(WATER_REFLECTION_SHADOW_RESOLUTION, WATER_REFLECTION_SHADOW_RESOLUTION, false),
		m_SceneReflectionFramebuffer(WATER_REFLECTION_RESOLUTION_WIDTH, WATER_REFLECTION_RESOLUTION_HEIGHT, false),
		m_SceneRefractionFramebuffer(WATER_REFRACTION_RESOLUTION_WIDTH, WATER_REFRACTION_RESOLUTION_HEIGHT, false),
		m_WaterPos(1095.0f, 83.0f, 730.0f), m_WaterScale(600.0f), m_EnableClearWater(false), m_WaveMoveFactor(0.0f), 
		m_WaveSpeed(0.05f), m_WaterAlbedo(0.1f, 0.9f, 0.9f), m_AlbedoPower(0.1f)
	{
		m_WaterShader = ShaderLoader::loadShader("src/shaders/Water.glsl");

		m_SceneShadowFramebuffer.addDepthStencilTexture(NormalizedDepthOnly).createFramebuffer();
		m_SceneReflectionFramebuffer.addColorTexture(FloatingPoint16).addDepthStencilRBO(NormalizedDepthOnly).createFramebuffer();
		m_SceneRefractionFramebuffer.addColorTexture(FloatingPoint16).addDepthStencilRBO(NormalizedDepthOnly).createFramebuffer();

		m_WaveTexture = TextureLoader::load2DTexture(std::string("res/water/dudv.png"));
		m_WaterNormalMap = TextureLoader::load2DTexture(std::string("res/water/normals.png"));

		m_Timer.reset();
	}

	WaterPass::~WaterPass()
	{	}

	OpenGL_Engine::WaterPassOutput WaterPass::executeWaterPass(LightingPassOutput& postTransprarency, ICamera* camera)
	{
		WaterPassOutput passOutput;
		if (!m_WaterEnabled) {
			passOutput.outputFramebuffer = postTransprarency.outputFramebuffer;
			return passOutput;
		}

		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		m_GLCache->setUsesClipPlane(true);
		
		//generate reflection framebuffer and render to it
		{
			m_GLCache->setClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -m_WaterPos.y));
			float distance = 2 * (camera->getPosition().y - m_WaterPos.y);
			camera->setPosition(camera->getPosition() - glm::vec3(0.0f, distance, 0.0f));
			camera->invertPitch();

			ShadowmapPass shadowPass(m_ActiveScene, &m_SceneShadowFramebuffer);
			ForwardLightingPass lightingPass(m_ActiveScene, &m_SceneReflectionFramebuffer);

			ShadowmapPassOutput shadowpassOutput = shadowPass.generateShadowmaps(camera, false);
			lightingPass.executeLightingPass(shadowpassOutput, camera, false, false);

			camera->setPosition(camera->getPosition() + glm::vec3(0.0f, distance, 0.0f));
			camera->invertPitch();
		}
		//generate refraction framebuffer and render to it
		{
			m_GLCache->setClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, m_WaterPos.y));

			ShadowmapPass shadowPass(m_ActiveScene, &m_SceneShadowFramebuffer);
			ForwardLightingPass lightingPass(m_ActiveScene, &m_SceneRefractionFramebuffer);

			ShadowmapPassOutput shadowpassOutput = shadowPass.generateShadowmaps(camera, false);
			lightingPass.executeLightingPass(shadowpassOutput, camera, false, false);
		}

		m_GLCache->setUsesClipPlane(false);
	}

}