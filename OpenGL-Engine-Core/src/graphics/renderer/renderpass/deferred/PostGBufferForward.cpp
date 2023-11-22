#include "pch.h"
#include "PostGBufferForward.h"

#include <utils/loaders/ShaderLoader.h>

namespace OpenGL_Engine {

	PostGBufferForward::PostGBufferForward(Scene3D* scene) : RenderPass(scene, RenderPassType::LightingPassType)
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/forward/pbr_model.vert", "src/shaders/forward/pbr_model.frag");
	}

	PostGBufferForward::~PostGBufferForward() {}

	LightingPassOutput PostGBufferForward::executeLightingPass(ShadowmapPassOutput& shadowmapData, LightingPassOutput& lightingPassData, ICamera* camera, bool renderOnlyStatic, bool useIBL) {
		glViewport(0, 0, lightingPassData.outputFramebuffer->getWidth(), lightingPassData.outputFramebuffer->getHeight());
		lightingPassData.outputFramebuffer->bind();
		m_GLCache->setMultisample(false);
		m_GLCache->setDepthTest(true);

		// Setup
		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		DynamicLightManager* lightManager = m_ActiveScene->getDynamicLightManager();
		Skybox* skybox = m_ActiveScene->getSkybox();
		ProbeManager* probeManager = m_ActiveScene->getProbeManager();

		// Render skybox
		skybox->Draw(camera);

		// View setup + lighting setup
		auto lightBindFunction = &DynamicLightManager::bindLightingUniforms;
		if (renderOnlyStatic)
			lightBindFunction = &DynamicLightManager::bindStaticLightingUniforms;

		m_GLCache->switchShader(m_ModelShader);
		(lightManager->*lightBindFunction) (m_ModelShader);
		m_ModelShader->setUniform3f("viewPos", camera->getPosition());
		m_ModelShader->setUniformMat4("view", camera->getViewMatrix());
		m_ModelShader->setUniformMat4("projection", camera->getProjectionMatrix());

		// Shadowmap code
		bindShadowmap(m_ModelShader, shadowmapData);

		// IBL code
		if (useIBL) {
			m_ModelShader->setUniform1i("computeIBL", 1);
			probeManager->bindProbes(glm::vec3(0.0f, 0.0f, 0.0f), m_ModelShader);
		}
		else {
			m_ModelShader->setUniform1i("computeIBL", 0);
		}

		// Render only transparent materials since we already rendered opaque using deferred
		if (renderOnlyStatic) {
			m_ActiveScene->addTransparentStaticModelsToRenderer();
		}
		else {
			m_ActiveScene->addTransparentModelsToRenderer();
		}

		// Render transparent objects
		modelRenderer->flushTransparent(m_ModelShader, m_RenderPassType);

		// Render pass output
		LightingPassOutput passOutput;
		passOutput.outputFramebuffer = lightingPassData.outputFramebuffer;
		return passOutput;
	}

	void PostGBufferForward::bindShadowmap(Shader* shader, ShadowmapPassOutput& shadowmapData) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowmapData.shadowmapFramebuffer->getDepthTexture());
		shader->setUniform1i("shadowmap", 0);
		shader->setUniformMat4("lightSpaceViewProjectionMatrix", shadowmapData.directionalLightViewProjMatrix);
	}

}
