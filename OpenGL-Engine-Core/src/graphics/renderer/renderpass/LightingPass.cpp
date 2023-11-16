#include "pch.h"
#include "LightingPass.h"

#include <utils/loaders/ShaderLoader.h>
namespace OpenGL_Engine {
	
	LightingPass::LightingPass(Scene3D* scene) 
		: RenderPass(scene, RenderPassType::LightingPassType)
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/pbr_model.vert", "src/shaders/pbr_model.frag");
		m_TerrainShader = ShaderLoader::loadShader("src/shaders/terrain.vert", "src/shaders/terrain.frag");

		m_Framebuffer = new FrameBuffer(Window::getWidth(), Window::getHeight());
		bool shouldMultisample = MSAA_SAMPLE_AMOUNT > 1.0 ? true : false;
		m_Framebuffer->addTexture2DColorAttachment(shouldMultisample).addDepthStencilRBO(shouldMultisample).createFramebuffer();
	}
	LightingPass::LightingPass(Scene3D* scene, FrameBuffer* customFramebuffer) : RenderPass(scene, RenderPassType::LightingPassType), m_Framebuffer(customFramebuffer)
	{
		m_ModelShader = ShaderLoader::loadShader("src/shaders/pbr_model.vert", "src/shaders/pbr_model.frag");
		m_TerrainShader = ShaderLoader::loadShader("src/shaders/terrain.vert", "src/shaders/terrain.frag");
	}

	LightingPass::~LightingPass()
	{ }

	OpenGL_Engine::LightingPassOutput LightingPass::executeRenderPass(ShadowmapPassOutput shadowmapData, ICamera* camera)
	{
		glViewport(0, 0, m_Framebuffer->getWidth(), m_Framebuffer->getHeight());
		m_Framebuffer->bind();
		m_Framebuffer->clear();

		// Setup
		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		Terrain* terrain = m_ActiveScene->getTerrain();
		DynamicLightManager* lightManager = m_ActiveScene->getDynamicLightManager();
		Skybox* skybox = m_ActiveScene->getSkybox();


		// Models
		m_GLCache->switchShader(m_ModelShader);
		lightManager->setupLightingUniforms(m_ModelShader);
		m_ModelShader->setUniform3f("viewPos", camera->getPosition());
		m_ModelShader->setUniformMat4("view", camera->getViewMatrix());
		m_ModelShader->setUniformMat4("projection", camera->getProjectionMatrix());
		
		/*
		lightManager->setSpotLightDirection(Camera->getFront());
		lightManager->setSpotLightPosition(Camera->getPosition());
		*/
		// Shadow map code
		m_ModelShader->setUniform1i("shadowmap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowmapData.shadowmapFramebuffer->getDepthTexture());
		m_ModelShader->setUniformMat4("lightSpaceViewProjectionMatrix", shadowmapData.directionalLightViewProjMatrix);

		// IBL code
		m_ModelShader->setUniform1i("irradianceMap", 1);
		skybox->getSkyboxCubemap()->bind(1);

		// Add objects to the renderer
		m_ActiveScene->addModelsToRenderer();
		// Opaque objects
		modelRenderer->flushOpaque(m_ModelShader, RenderPassType::LightingPassType);

		// Terrain
		m_GLCache->switchShader(m_TerrainShader->getShaderID());
		m_TerrainShader->setUniform1i("shadowmap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowmapData.shadowmapFramebuffer->getDepthTexture());

		lightManager->setupLightingUniforms(m_TerrainShader);
		m_TerrainShader->setUniform3f("viewPos", camera->getPosition());
		glm::mat4 modelMatrix(1);
		modelMatrix = glm::translate(modelMatrix, terrain->getPosition());
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
		m_TerrainShader->setUniformMat3("normalMatrix", normalMatrix);
		m_TerrainShader->setUniformMat4("model", modelMatrix);
		m_TerrainShader->setUniformMat4("view", camera->getViewMatrix());
		m_TerrainShader->setUniformMat4("projection", camera->getProjectionMatrix());
		m_TerrainShader->setUniformMat4("lightSpaceViewProjectionMatrix", shadowmapData.directionalLightViewProjMatrix);
		terrain->Draw(m_TerrainShader, RenderPassType::LightingPassType);

		// Skybox
		skybox->Draw(camera);

		// Transparent objects
		m_GLCache->switchShader(m_ModelShader->getShaderID());
		modelRenderer->flushTransparent(m_ModelShader, RenderPassType::LightingPassType);

		// Render pass output
		LightingPassOutput passOutput;
		passOutput.outputFramebuffer = m_Framebuffer;
		return passOutput;
	}

}