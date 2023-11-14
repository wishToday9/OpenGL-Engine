#include <Scene/Scene3D.h>
#include <graphics/Shader.h>
#include <graphics/Window.h>
#include <graphics/camera/FPSCamera.h>
#include <graphics/mesh/Model.h>
#include <graphics/mesh/common/Quad.h>
#include <graphics/renderer/GLCache.h>
#include <graphics/renderer/PostProcessor.h>
#include <platform/OpenGL/Framebuffers/Framebuffer.h>
#include <terrain/Terrain.h>
#include <ui/DebugPane.h>
#include <ui/RuntimePane.h>
#include <utils/Time.h>
#include <utils/Timer.h>


int main() {
	// Prepare the engine
	OpenGL_Engine::FPSCamera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	OpenGL_Engine::Window window("OpenGL_Engine Engine", WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
	OpenGL_Engine::Scene3D scene(&camera, &window);
	OpenGL_Engine::GLCache* glCache = OpenGL_Engine::GLCache::getInstance();
	OpenGL_Engine::PostProcessor postProcessor(scene.getRenderer());

	OpenGL_Engine::TextureLoader::initializeDefaultTextures();

	// Prepare the UI
	OpenGL_Engine::RuntimePane runtimePane(glm::vec2(256.0f, 90.0f));
	OpenGL_Engine::DebugPane debugPane(glm::vec2(256.0f, 100.0f));



	// Construct framebuffers
	bool shouldMultisample = MSAA_SAMPLE_AMOUNT > 1.0 ? true : false;
	OpenGL_Engine::FrameBuffer framebuffer(window.getWidth(), window.getHeight());
	framebuffer.addTexture2DColorAttachment(shouldMultisample).addDepthStencilRBO(shouldMultisample).createFramebuffer();
	// TODO: MAKE MULTISAMPLE OPTION WORK OR INVESTIGATE
	OpenGL_Engine::FrameBuffer shadowmap(SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
	shadowmap.addDepthAttachment(false).createFramebuffer();

	// Debug timers
#if DEBUG_ENABLED
	OpenGL_Engine::Timer timer;
#endif

	OpenGL_Engine::Time deltaTime;
	while (!window.closed()) {
		deltaTime.update();

#if DEBUG_ENABLED
		if (debugPane.getWireframeMode())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

		window.clear();
		ImGui_ImplGlfwGL3_NewFrame();

		// Shadowmap Pass
#if DEBUG_ENABLED
		glFinish();
		timer.reset();
#endif
		glViewport(0, 0, shadowmap.getWidth(), shadowmap.getHeight());
		shadowmap.bind();
		shadowmap.clear();
		scene.shadowmapPass();
#if DEBUG_ENABLED
		glFinish();
		runtimePane.setShadowmapTimer(timer.elapsed());
#endif

		// Camera Update
		camera.processInput(deltaTime.getDeltaTime());

		// Draw the scene to our custom multisampled framebuffer
		glViewport(0, 0, framebuffer.getWidth(), framebuffer.getHeight());
		framebuffer.bind();
		framebuffer.clear();
		scene.onUpdate(deltaTime.getDeltaTime());
		scene.onRender(shadowmap.getDepthTexture());

		// Perform post processing
		postProcessor.postLightingPostProcess(&framebuffer);

		// Display panes
		runtimePane.render();
		debugPane.render();


		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();

		if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window.getNativeWindow(), true);
	}
	return 0;
}
//int main() {
//	// Prepare the game
//	OpenGL_Engine::Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
//	OpenGL_Engine::Window window("OpenGL Engine", WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
//	OpenGL_Engine::Scene3D scene(&camera, &window);
//	OpenGL_Engine::GLCache* glCache = OpenGL_Engine::GLCache::getInstance();
//	OpenGL_Engine::TextureLoader::initializeDefaultTextures();
//
//	// Construct framebuffers
//	OpenGL_Engine::RenderTarget framebuffer(window.getWidth(), window.getHeight());
//	framebuffer.addColorAttachment(true).addDepthStencilRBO(true).createFramebuffer();
//	// TODO: MAKE MULTISAMPLE OPTION WORK OR INVESTIGATE
//	OpenGL_Engine::RenderTarget shadowmap(SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
//	shadowmap.addDepthAttachment(false).createFramebuffer();
//	OpenGL_Engine::RenderTarget blitFramebuffer(window.getWidth(), window.getHeight());
//	blitFramebuffer.addColorAttachment(false).addDepthStencilRBO(false).createFramebuffer();
//
//	// Instantiate the shaders and mesh factories
//	OpenGL_Engine::Shader framebufferShader("src/shaders/postprocess.vert", "src/shaders/postprocess.frag");
//	OpenGL_Engine::Quad screenQuad;
//	screenQuad.getMaterial().setDiffuseMap(blitFramebuffer.getColourBufferTexture());
//
//	// Setup post processing information
//	glCache->switchShader(framebufferShader.getShaderID());
//	framebufferShader.setUniform2f("readOffset", glm::vec2(1.0f / (float)window.getWidth(), 1.0f / (float)window.getHeight()));
//
//	bool wireframeMode = false;
//
//	// Debug timers
//#if DEBUG_ENABLED
//	OpenGL_Engine::Timer timer;
//	float postProcessTime = 0.0f, shadowmapGenerationTime = 0.0f;
//#endif
//
//	framebufferShader.enable();
//	framebufferShader.setUniform2f("readOffset", glm::vec2(1.0f / (float)window.getWidth(), 1.0f / (float)window.getHeight()));
//
//	OpenGL_Engine::Time deltaTime;
//	while (!window.closed()) {
//		deltaTime.update();
//#if DEBUG_ENABLED
//		if (wireframeMode) {
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		}
//		else {
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		}
//#endif
//
//		window.clear();
//		ImGui_ImplGlfwGL3_NewFrame();
//
//		// Shadowmap Pass
//#if DEBUG_ENABLED
//		glFinish();
//		timer.reset();
//#endif
//		glViewport(0, 0, shadowmap.getWidth(), shadowmap.getHeight());
//		shadowmap.bind();
//		shadowmap.clear();
//		scene.shadowmapPass();
//
//#if DEBUG_ENABLED
//		glFinish();
//		shadowmapGenerationTime = timer.elapsed();
//#endif
//
//		// Camera Update
//		camera.processInput(deltaTime.getDeltaTime());
//
//		// Draw the scene to our custom multisampled framebuffer
//		glViewport(0, 0, framebuffer.getWidth(), framebuffer.getHeight());
//		framebuffer.bind();
//		framebuffer.clear();
//		scene.onUpdate(deltaTime.getDeltaTime());
//		scene.onRender(shadowmap.getDepthTexture());
//
//		// Blit the multisampled framebuffer over to a non-multisampled buffer and perform a post process pass on the default framebuffer
//#if DEBUG_ENABLED
//		glFinish();
//		timer.reset();
//#endif
//		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.getFramebuffer());
//		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, blitFramebuffer.getFramebuffer());
//		glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), 0, 0, window.getWidth(), window.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
//
//#if DEBUG_ENABLED
//		if (wireframeMode) {
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		}
//#endif
//		
//		framebuffer.unbind();
//		window.clear();
//		glCache->switchShader(framebufferShader.getShaderID());
//		screenQuad.getMaterial().BindMaterialInformation(framebufferShader);
//		screenQuad.Draw();
//#if DEBUG_ENABLED
//		glFinish();
//		postProcessTime = timer.elapsed();
//#endif
//
//		// Create an ImGui analytics window
//		{
//			ImGui::Begin("Runtime Analytics", nullptr, ImVec2(100.0f, 50.0f));
//			ImGui::Text("Frametime: %.3f ms (FPS %.1f)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//#if DEBUG_ENABLED
//			ImGui::Text("Post Process: %.6f ms", 1000.0f * postProcessTime);
//			ImGui::Text("Shadowmap Generation: %.6f ms", 1000.0f * shadowmapGenerationTime);
//#endif
//			ImGui::End();
//
//#if DEBUG_ENABLED
//			ImGui::Begin("Debug Controls", nullptr, ImVec2(100.0f, 150.0f));
//			ImGui::Text("Hit \"P\" to show/hide the cursor");
//			ImGui::Checkbox("Wireframe Mode", &wireframeMode);
//			ImGui::End();
//#endif
//		}
//		ImGui::Render();
//		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
//
//		window.resetScroll();
//		window.update();
//	}
//	return 0;
//}