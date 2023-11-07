#include <iostream>
#include "graphics\Window.h"
#include "utils\Timer.h"
#include "graphics\Shader.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils\Time.h"
#include "graphics\camera\Camera.h"
#include "utils\Logger.h"
#include "graphics\mesh\Model.h"
#include "terrain\Terrain.h"
#include "Scene3D.h"
#include "platform\OpenGL\Framebuffers\Framebuffer.h"
#include "graphics/mesh/common/Quad.h"
#include "graphics/renderer/GLCache.h"


int main() {
	// Prepare the game
	OpenGL_Engine::graphics::Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	OpenGL_Engine::graphics::Window window("Arcane Engine", WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
	OpenGL_Engine::Scene3D scene(&camera, &window);
	OpenGL_Engine::graphics::GLCache* glCache = OpenGL_Engine::graphics::GLCache::getInstance();
	OpenGL_Engine::utils::TextureLoader::InitializeDefaultTextures();

	// Construct framebuffers
	OpenGL_Engine::opengl::Framebuffer framebuffer(window.getWidth(), window.getHeight());
	framebuffer.addColorAttachment(true).addDepthStencilRBO(true).createFramebuffer();
	OpenGL_Engine::opengl::Framebuffer blitFramebuffer(window.getWidth(), window.getHeight());
	blitFramebuffer.addColorAttachment(false).addDepthStencilRBO(false).createFramebuffer();

	// Instantiate the shaders and mesh factories
	OpenGL_Engine::graphics::Shader framebufferShader("src/shaders/postprocess.vert", "src/shaders/postprocess.frag");
	OpenGL_Engine::graphics::Quad screenQuad;
	screenQuad.getMaterial().setDiffuseMap(blitFramebuffer.getColourBufferTexture());

	// Setup post processing information
	glCache->switchShader(framebufferShader.getShaderID());
	framebufferShader.setUniform2f("readOffset", glm::vec2(1.0f / (float)window.getWidth(), 1.0f / (float)window.getHeight()));

	// Debug timers
#if DEBUG_ENABLED
	OpenGL_Engine::Timer timer;
	float postProcessTime = 0.0f;
#endif

	framebufferShader.enable();
	framebufferShader.setUniform2f("readOffset", glm::vec2(1.0f / (float)window.getWidth(), 1.0f / (float)window.getHeight()));

	OpenGL_Engine::Time deltaTime;
	while (!window.closed()) {
		deltaTime.update();

		window.clear();
		ImGui_ImplGlfwGL3_NewFrame();

		// Camera Update
		camera.processInput(deltaTime.getDeltaTime());

		// Draw the scene to our custom multisampled framebuffer
		framebuffer.bind();
		window.clear();
		scene.onUpdate(deltaTime.getDeltaTime());
		scene.onRender();

		// Blit the multisampled framebuffer over to a non-multisampled buffer and perform a post process pass on the default framebuffer
#if DEBUG_ENABLED
		glFinish();
		timer.reset();
#endif
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.getFramebuffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, blitFramebuffer.getFramebuffer());
		glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), 0, 0, window.getWidth(), window.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
		framebuffer.unbind();
		window.clear();
		glCache->switchShader(framebufferShader.getShaderID());
		screenQuad.getMaterial().BindMaterialInformation(framebufferShader);
		screenQuad.Draw();
#if DEBUG_ENABLED
		glFinish();
		postProcessTime = timer.elapsed();
#endif

		// Create an ImGui analytics window
		{
			ImGui::Begin("Runtime Analytics", nullptr, ImVec2(100.0f, 50.0f));
			ImGui::Text("Frametime: %.3f ms (FPS %.1f)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
#if DEBUG_ENABLED
			ImGui::Text("Post Process: %.6f ms", 1000.0f * postProcessTime);
#endif
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.resetScroll();
		window.update();
	}
	return 0;
}