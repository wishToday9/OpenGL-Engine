#include "pch.h"
#include "ShadowmapPass.h"

#include <platform/OpenGL/Framebuffers/FrameBuffer.h>
#include <utils/loaders/ShaderLoader.h>


namespace OpenGL_Engine {
	ShadowmapPass::ShadowmapPass(Scene3D* scene)
		:RenderPass(scene, RenderPassType::ShadowmapPassType), m_AllocateFramebuffer(true)
	{
		m_ShadowmapShader = ShaderLoader::loadShader("src/shaders/shadowmap.vert", "src/shaders/shadowmap.frag");
		m_ShadowmapFramebuffer = new FrameBuffer(SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
		m_ShadowmapFramebuffer->addDepthAttachment(false).createFramebuffer();
	}

	ShadowmapPass::ShadowmapPass(Scene3D* scene, FrameBuffer* customFramebuffer)
		: RenderPass(scene, RenderPassType::ShadowmapPassType), 
		m_ShadowmapFramebuffer(customFramebuffer), m_AllocateFramebuffer(false)
	{
		m_ShadowmapShader = ShaderLoader::loadShader("src/shaders/shadowmap.vert", "src/shaders/shadowmap.frag");
	}

	ShadowmapPass::~ShadowmapPass()
	{	
		if (m_AllocateFramebuffer) {
			SAFE_DELETE(m_ShadowmapShader);
		}
	}

	OpenGL_Engine::ShadowmapPassOutput ShadowmapPass::generateShadowmaps(ICamera* camera, bool renderOnlyStatic)
	{
		glViewport(0, 0, m_ShadowmapFramebuffer->getWidth(), m_ShadowmapFramebuffer->getHeight());
		m_ShadowmapFramebuffer->bind();
		m_ShadowmapFramebuffer->clear();

		//setup 
		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		Terrain* terrain = m_ActiveScene->getTerrain();
		DynamicLightManager* lightManager = m_ActiveScene->getDynamicLightManager();


		// Temporary location code for the shadowmap. Will move to a proper system with CSM (Cascaded shadow maps)
		m_GLCache->switchShader(m_ShadowmapShader->getShaderID());
		glm::vec3 dirLightShadowmapLookAtPos = camera->getPosition() + (glm::normalize(camera->getFront()) * 50.0f);
		glm::vec3 dirLightShadowmapEyePos = dirLightShadowmapLookAtPos + (-lightManager->getDirectionalLightDirection(0) * 100.0f);

		glm::mat4 directionalLightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, SHADOWMAP_NEAR_PLANE, SHADOWMAP_FAR_PLANE);
		glm::mat4 directionalLightView = glm::lookAt(dirLightShadowmapEyePos, dirLightShadowmapLookAtPos, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 directionalLightViewProjMatrix = directionalLightProjection * directionalLightView;
		m_ShadowmapShader->setUniformMat4("lightSpaceViewProjectionMatrix", directionalLightViewProjMatrix);

		//render models
		if (renderOnlyStatic) {
			m_ActiveScene->addStaticModelsToRenderer();
		}
		else {
			m_ActiveScene->addModelsToRenderer();
		}

		modelRenderer->flushOpaque(m_ShadowmapShader, RenderPassType::ShadowmapPassType);
		modelRenderer->flushTransparent(m_ShadowmapShader, RenderPassType::ShadowmapPassType);
		terrain->Draw(m_ShadowmapShader, m_RenderPassType);
		
		//render pass output
		ShadowmapPassOutput passOutput;
		passOutput.directionalLightViewProjMatrix = directionalLightViewProjMatrix;
		passOutput.shadowmapFramebuffer = m_ShadowmapFramebuffer;
		return passOutput;
	}

}