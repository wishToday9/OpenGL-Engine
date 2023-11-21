#include "pch.h"
#include "ForwardLightingPass.h"

#include <utils/loaders/ShaderLoader.h>
namespace OpenGL_Engine {
	
	ForwardLightingPass::ForwardLightingPass(Scene3D* scene, bool shouldMultisample)
		: RenderPass(scene, RenderPassType::LightingPassType), m_AllocatedFramebuffer(true)
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/forward/pbr_model.vert", "src/shaders/forward/pbr_model.frag");
		m_TerrainShader = ShaderLoader::loadShader("src/shaders/forward/pbr_terrain.vert", "src/shaders/forward/pbr_terrain.frag");

		m_Framebuffer = new FrameBuffer(Window::getWidth(), Window::getHeight());
		m_Framebuffer->addTexture2DColorAttachment(shouldMultisample).addDepthStencilRBO(shouldMultisample).createFramebuffer();
	}
	ForwardLightingPass::ForwardLightingPass(Scene3D* scene, FrameBuffer* customFramebuffer) 
		: RenderPass(scene, RenderPassType::LightingPassType), m_Framebuffer(customFramebuffer), m_AllocatedFramebuffer(false)
		
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/forward/pbr_model.vert", "src/shaders/forward/pbr_model.frag");
		m_TerrainShader = ShaderLoader::loadShader("src/shaders/forward/pbr_terrain.vert", "src/shaders/forward/pbr_terrain.frag");
	}

	ForwardLightingPass::~ForwardLightingPass()
	{ 
		if (m_AllocatedFramebuffer) {
			SAFE_DELETE(m_Framebuffer);
		}
		
	}

	OpenGL_Engine::LightingPassOutput ForwardLightingPass::executeRenderPass(ShadowmapPassOutput& shadowmapData, ICamera* camera, bool renderOnlyStatic, bool useIBL)
	{
		glViewport(0, 0, m_Framebuffer->getWidth(), m_Framebuffer->getHeight());
		m_Framebuffer->bind();
		m_Framebuffer->clear();

		// view setup + lighting setup
		auto lightBindFuncion = &DynamicLightManager::bindLightingUniforms;
		if (renderOnlyStatic) {
			lightBindFuncion = &DynamicLightManager::bindStaticLightingUniforms;
		}

		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		Terrain* terrain = m_ActiveScene->getTerrain();
		DynamicLightManager* lightManager = m_ActiveScene->getDynamicLightManager();
		Skybox* skybox = m_ActiveScene->getSkybox();

		ProbeManager* probeManager = m_ActiveScene->getProbeManager();

		// Models
		m_GLCache->switchShader(m_ModelShader);
		(lightManager->*lightBindFuncion)(m_ModelShader);
		m_ModelShader->setUniform3f("viewPos", camera->getPosition());
		m_ModelShader->setUniformMat4("view", camera->getViewMatrix());
		m_ModelShader->setUniformMat4("projection", camera->getProjectionMatrix());
		
		/*
		lightManager->setSpotLightDirection(Camera->getFront());
		lightManager->setSpotLightPosition(Camera->getPosition());
		*/
		// Shadow map code
		bindShadowmap(m_ModelShader, shadowmapData);

		// IBL code
		if (useIBL) {
			m_ModelShader->setUniform1i("computeIBL", 1);
			probeManager->bindProbe(glm::vec3(0.0, 0.0, 0.0), m_ModelShader);
		}
		else {
			m_ModelShader->setUniform1i("computeIBL", 0);
		}

		//setup model renderer
		if (renderOnlyStatic) {
			m_ActiveScene->addStaticModelsToRenderer();
		}
		else {
			m_ActiveScene->addModelsToRenderer();
		}

		// Setup model renderer
		if (renderOnlyStatic) {
			m_ActiveScene->addStaticModelsToRenderer();
		}
		else {
			m_ActiveScene->addModelsToRenderer();
		}
		// Opaque objects
		modelRenderer->flushOpaque(m_ModelShader, RenderPassType::LightingPassType);

		// Terrain
		m_GLCache->switchShader(m_TerrainShader->getShaderID());

		(lightManager->*lightBindFuncion)(m_TerrainShader);
		m_TerrainShader->setUniform3f("viewPos", camera->getPosition());

		m_TerrainShader->setUniformMat4("view", camera->getViewMatrix());
		m_TerrainShader->setUniformMat4("projection", camera->getProjectionMatrix());
		
		bindShadowmap(m_TerrainShader, shadowmapData);
		terrain->Draw(m_TerrainShader, RenderPassType::LightingPassType);

		//render Skybox
		skybox->Draw(camera);

		//render Transparent objects
		m_GLCache->switchShader(m_ModelShader->getShaderID());
		modelRenderer->flushTransparent(m_ModelShader, RenderPassType::LightingPassType);

		// Render pass output
		LightingPassOutput passOutput;
		passOutput.outputFramebuffer = m_Framebuffer;
		return passOutput;
	}

	void ForwardLightingPass::bindShadowmap(Shader* shader, ShadowmapPassOutput& shadowmapData)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowmapData.shadowmapFramebuffer->getDepthTexture());
		shader->setUniform1i("shadowmap", 0);
		shader->setUniformMat4("lightSpaceViewProjectionMatrix", shadowmapData.directionalLightViewProjMatrix);

	}

}