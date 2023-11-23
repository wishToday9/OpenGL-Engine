#include "pch.h"
#include "PostProcessPass.h"

#include <utils/loaders/ShaderLoader.h>


namespace OpenGL_Engine
{

	PostProcessPass::PostProcessPass(Scene3D* scene) : RenderPass(scene), m_SsaoRenderTarget(Window::getResolutionWidth(), Window::getResolutionHeight(), false), m_TonemappedNonLinearTarget(Window::getWidth(), Window::getHeight(), false),
		m_ScreenRenderTarget(Window::getWidth(), Window::getHeight(), false), m_ResolveRenderTarget(Window::getResolutionWidth(), Window::getResolutionHeight(), false)
	{
		//shader setup
		m_PostProcessShader = ShaderLoader::loadShader("src/shaders/postprocess.vert", "src/shaders/postprocess.frag");
		m_FxaaShader = ShaderLoader::loadShader("src/shaders/fxaa.vert", "src/shaders/fxaa.frag");
		m_SsaoShader = ShaderLoader::loadShader("src/shaders/ssao.vert", "src/shaders/ssao.frag");

		//framebuffer setup
		m_SsaoRenderTarget.addColorTexture(NormalizedSingleChannel8).createFramebuffer();
		m_TonemappedNonLinearTarget.addColorTexture(Normalized8).addDepthStencilRBO(NormalizedDepthOnly).createFramebuffer();
		m_ScreenRenderTarget.addColorTexture(FloatingPoint16).addDepthStencilRBO(NormalizedDepthOnly).createFramebuffer();
		m_ResolveRenderTarget.addColorTexture(FloatingPoint16).addDepthStencilRBO(NormalizedDepthOnly).createFramebuffer();
		
		//SSAO 
		std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
		std::default_random_engine generator;
		for (unsigned int i = 0; i < m_SsaoKernel.size(); ++i) {
			glm::vec3 hemisphereSample = glm::vec3((randomFloats(generator) * 2.0f) - 1.0f, 
				(randomFloats(generator) * 2.0f) - 1.0f, randomFloats(generator));
			hemisphereSample = glm::normalize(hemisphereSample);

			//generate more samples closer to the origin of the hemisphere.
			//since these make for better light occlusion tests
			float scale = (float)i / m_SsaoKernel.size();
			scale = lerp(0.1f, 1.0f, scale * scale);
			hemisphereSample *= scale;

			m_SsaoKernel[i] = hemisphereSample;
		}
		
		//SSAO Random Rotation texture
		std::array<glm::vec3, 16> noiseSSAO;
		for (unsigned int i = 0; i < noiseSSAO.size(); i++) {
			noiseSSAO[i] = glm::vec3((randomFloats(generator) * 2.0f) - 1.0f, (randomFloats(generator) * 2.0f) - 1.0f, 0.0f);
		}
		TextureSettings ssaoNoiseTextureSettings;
		ssaoNoiseTextureSettings.TextureFormat = GL_RGB16F;
		ssaoNoiseTextureSettings.TextureWrapSMode = GL_REPEAT;
		ssaoNoiseTextureSettings.TextureWrapTMode = GL_REPEAT;
		ssaoNoiseTextureSettings.TextureMinificationFilterMode = GL_NEAREST;
		ssaoNoiseTextureSettings.TextureMagnificationFilterMode = GL_NEAREST;
		ssaoNoiseTextureSettings.TextureAnisotropyLevel = 1.0f;
		ssaoNoiseTextureSettings.HasMips = false;
		m_SsaoNoiseTexture.setTextureSettings(ssaoNoiseTextureSettings);
		m_SsaoNoiseTexture.generate2DTexture(4, 4, GL_RGB, &noiseSSAO[0], GL_FLOAT);

		DebugPane::bindGammaCorrectionValue(&m_GammaCorrection);
		DebugPane::bindSsaoSampleRadiusValue(&m_SsaoSampleRadius);
	}

	PostProcessPass::~PostProcessPass() {	}


	// Generates the AO of the scene using SSAO and stores it in a single channel texture
	PreLightingPassOutput PostProcessPass::executePreLightingPass(GeometryPassOutput& geometryData, ICamera* camera)
	{
		glViewport(0, 0, m_SsaoRenderTarget.getWidth(), m_SsaoRenderTarget.getHeight());
		m_SsaoRenderTarget.bind();
		m_GLCache->setDepthTest(false);
		m_GLCache->setFaceCull(true);
		m_GLCache->setCullFace(GL_BACK);

		// Setup
		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();

		// Bind the required data to perform SSAO
		m_GLCache->switchShader(m_SsaoShader);

		// Used to tile the noise texture across the screen every 4 texels (because our noise texture is 4x4)
		m_SsaoShader->setUniform2f("noiseScale", glm::vec2(m_SsaoRenderTarget.getWidth() / 4.0f, m_SsaoRenderTarget.getHeight() / 4.0f));

		m_SsaoShader->setUniform1i("numKernelSamples", SSAO_KERNEL_SIZE);
		for (unsigned int i = 0; i < SSAO_KERNEL_SIZE; i++) {
			m_SsaoShader->setUniform3f(("samples[" + std::to_string(i) + "]").c_str(), m_SsaoKernel[i]);
		}

		m_SsaoShader->setUniformMat4("view", camera->getViewMatrix());
		m_SsaoShader->setUniformMat4("projection", camera->getProjectionMatrix());
		m_SsaoShader->setUniformMat4("viewInverse", glm::inverse(camera->getViewMatrix()));
		m_SsaoShader->setUniformMat4("projectionInverse", glm::inverse(camera->getProjectionMatrix()));

		geometryData.outputGBuffer->getNormal()->bind(0);
		m_SsaoShader->setUniform1i("normalTexture", 0);
		geometryData.outputGBuffer->getDepthStencilTexture()->bind(1);
		m_SsaoShader->setUniform1i("depthTexture", 1);
		m_SsaoNoiseTexture.bind(2);
		m_SsaoShader->setUniform1i("texNoise", 2);

		// Render our NDC quad to perform SSAO
		modelRenderer->NDC_Plane.Draw();

		// Reset unusual state
		m_GLCache->setDepthTest(true);

		// Render pass output
		PreLightingPassOutput passOutput;
		passOutput.ssaoFramebuffer = &m_SsaoRenderTarget;
		return passOutput;


	}

	void PostProcessPass::executePostProcessPass(Framebuffer* framebufferToProcess) {

		glViewport(0, 0, m_ScreenRenderTarget.getWidth(), m_ScreenRenderTarget.getHeight());

		//if the buffer is multi-sampled, resolve it
		Framebuffer* supersampledTarget = framebufferToProcess;
		if (framebufferToProcess->isMultisampled()) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferToProcess->getFramebuffer());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ResolveRenderTarget.getFramebuffer());
			glBlitFramebuffer(0, 0, framebufferToProcess->getWidth(), framebufferToProcess->getHeight(), 0, 0,
				m_ResolveRenderTarget.getWidth(), m_ResolveRenderTarget.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
			supersampledTarget = &m_ResolveRenderTarget;
		}

		// If some sort of super-sampling is set, we need to downsample (or upsample) our image to match the window's resolution
		Framebuffer* target = supersampledTarget;
		if (target->getWidth() != m_ScreenRenderTarget.getWidth() || target->getHeight() != m_ScreenRenderTarget.getHeight()) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, supersampledTarget->getFramebuffer());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ScreenRenderTarget.getFramebuffer());
			glBlitFramebuffer(0, 0, supersampledTarget->getWidth(), supersampledTarget->getHeight(), 0, 0, m_ScreenRenderTarget.getWidth(), m_ScreenRenderTarget.getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
			target = &m_ScreenRenderTarget;
		}


#if DEBUG_ENABLED
		if (DebugPane::getWireframeMode()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
#endif
		// Set post process settings and convert our scene from HDR (linear) -> SDR (non-linear)
		m_TonemappedNonLinearTarget.bind();
		m_TonemappedNonLinearTarget.clear();

		GLCache::getInstance()->switchShader(m_PostProcessShader);
		m_PostProcessShader->setUniform1f("gamma_inverse", 1.0f / m_GammaCorrection);
		m_PostProcessShader->setUniform2f("read_offset", glm::vec2(1.0f / (float)target->getWidth(), 1.0f / (float)target->getHeight()));
		m_PostProcessShader->setUniform1i("color_texture", 0);
		target->getColourTexture()->bind(0);

		Window::clear();
		ModelRenderer* modelRenderer = m_ActiveScene->getModelRenderer();
		modelRenderer->NDC_Plane.Draw();


		// Finally render the scene to the widnow's framebuffer
		m_TonemappedNonLinearTarget.unbind();
		Window::clear();
		GLCache::getInstance()->switchShader(m_FxaaShader);
		m_FxaaShader->setUniform1i("enable_FXAA", FXAA_ENABLE);
		m_FxaaShader->setUniform2f("inverse_resolution", glm::vec2(1.0f / (float)target->getWidth(), 1.0f / (float)target->getHeight()));

		m_FxaaShader->setUniform1i("colour_texture", 0);
		m_TonemappedNonLinearTarget.getColourTexture()->bind(0);

		modelRenderer->NDC_Plane.Draw();
	}

}