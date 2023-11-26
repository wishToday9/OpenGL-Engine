#include "pch.h"
#include "ForwardLightingPass.h"

#include <utils/loaders/ShaderLoader.h>
namespace OpenGL_Engine {
	
	ForwardLightingPass::ForwardLightingPass(Scene3D* scene, bool shouldMultisample)
		: RenderPass(scene), m_AllocatedFramebuffer(true)
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/forward/PBR_Model.glsl");
		m_TerrainShader = ShaderLoader::loadShader("src/shaders/forward/PBR_Terrain.glsl");

		m_Framebuffer = new Framebuffer(Window::getRenderResolutionWidth(), Window::getRenderResolutionHeight(), shouldMultisample);
		m_Framebuffer->addColorTexture(FloatingPoint16).addDepthStencilRBO(NormalizedDepthStencil).createFramebuffer();
	}
	ForwardLightingPass::ForwardLightingPass(Scene3D* scene, Framebuffer* customFramebuffer) 
		: RenderPass(scene), m_Framebuffer(customFramebuffer), m_AllocatedFramebuffer(false)
		
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/forward/PBR_Model.glsl");
		m_TerrainShader = ShaderLoader::loadShader("src/shaders/forward/PBR_Terrain.glsl");
	}

	ForwardLightingPass::~ForwardLightingPass()
	{ 
		if (m_AllocatedFramebuffer) {
			SAFE_DELETE(m_Framebuffer);
		}
		
	}

	OpenGL_Engine::LightingPassOutput ForwardLightingPass::executeLightingPass(ShadowmapPassOutput& shadowmapData, ICamera* camera, bool renderOnlyStatic, bool useIBL)
	{
		glViewport(0, 0, m_Framebuffer->getWidth(), m_Framebuffer->getHeight());
		m_Framebuffer->bind();
		m_Framebuffer->clear();
		
		if (m_Framebuffer->isMultisampled()) {
			m_GLCache->setMultisample(true);
		}
		else {
			m_GLCache->setMultisample(false);
		}

		//set up
		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		Terrain* terrain = m_ActiveScene->getTerrain();
		DynamicLightManager* lightManager = m_ActiveScene->getDynamicLightManager();
		Skybox* skybox = m_ActiveScene->getSkybox();
		ProbeManager* probeManager = m_ActiveScene->getProbeManager();

		// view setup + lighting setup
		auto lightBindFuncion = &DynamicLightManager::bindLightingUniforms;
		if (renderOnlyStatic) {
			lightBindFuncion = &DynamicLightManager::bindStaticLightingUniforms;
		}


		// Models
		m_GLCache->switchShader(m_ModelShader);
		(lightManager->*lightBindFuncion)(m_ModelShader);
		m_ModelShader->setUniform("viewPos", camera->getPosition());
		m_ModelShader->setUniform("view", camera->getViewMatrix());
		m_ModelShader->setUniform("projection", camera->getProjectionMatrix());
		
		/*
		lightManager->setSpotLightDirection(Camera->getFront());
		lightManager->setSpotLightPosition(Camera->getPosition());
		*/
		// Shadow map code
		bindShadowmap(m_ModelShader, shadowmapData);

		// IBL Binding
		probeManager->bindProbes(glm::vec3(0.0f, 0.0f, 0.0f), m_ModelShader);

		//setup model renderer
		if (renderOnlyStatic) {
			m_ActiveScene->addStaticModelsToRenderer();
		}
		else {
			m_ActiveScene->addModelsToRenderer();
		}

		// Render opaque objects
		if (useIBL) {
			m_ModelShader->setUniform("computeIBL", 1);
		}
		else {
			m_ModelShader->setUniform("computeIBL", 0);
		}

		// Opaque objects
		modelRenderer->setupOpaqueRenderState();
		modelRenderer->flushOpaque(m_ModelShader, RenderPassType::MaterialRequired);

		// Terrain
		m_GLCache->switchShader(m_TerrainShader->getShaderID());

		(lightManager->*lightBindFuncion)(m_TerrainShader);
		m_TerrainShader->setUniform("viewPos", camera->getPosition());

		m_TerrainShader->setUniform("view", camera->getViewMatrix());
		m_TerrainShader->setUniform("projection", camera->getProjectionMatrix());
		
		bindShadowmap(m_TerrainShader, shadowmapData);
		terrain->Draw(m_TerrainShader, RenderPassType::MaterialRequired);

		//render Skybox
		skybox->Draw(camera);

		//render Transparent objects
		m_GLCache->switchShader(m_ModelShader->getShaderID());
		if (useIBL) {
			probeManager->bindProbes(glm::vec3(0.0f, 0.0f, 0.0f), m_ModelShader);
		}
		
		modelRenderer->setupTransparentRenderState();
		modelRenderer->flushTransparent(m_ModelShader, RenderPassType::MaterialRequired);

		// Render pass output
		LightingPassOutput passOutput;
		passOutput.outputFramebuffer = m_Framebuffer;
		return passOutput;
	}

	void ForwardLightingPass::bindShadowmap(Shader* shader, ShadowmapPassOutput& shadowmapData)
	{
		shadowmapData.shadowmapFramebuffer->getDepthStencilTexture()->bind();
		shader->setUniform("shadowmap", 0);
		shader->setUniform("lightSpaceViewProjectionMatrix", shadowmapData.directionalLightViewProjMatrix);

	}

}