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
#include <graphics/renderer/MasterRenderer.h>


int main() {
	// Prepare the engine
	OpenGL_Engine::Window window("OpenGL_Engine Engine", WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
	OpenGL_Engine::Scene3D scene(&window);
	OpenGL_Engine::MasterRenderer renderer(&scene);

	OpenGL_Engine::TextureLoader::initializeDefaultTextures();

	// Prepare the UI
	OpenGL_Engine::RuntimePane runtimePane(glm::vec2(256.0f, 90.0f));
	OpenGL_Engine::DebugPane debugPane(glm::vec2(256.0f, 100.0f));


	// Debug timers
#if DEBUG_ENABLED
	OpenGL_Engine::Timer timer;
#endif

	OpenGL_Engine::Time deltaTime;
	while (!window.closed()) {
		deltaTime.update();

#if DEBUG_ENABLED
		if (debugPane.getWireframeMode()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
#endif

		window.clear();
		ImGui_ImplGlfwGL3_NewFrame();

		scene.onUpdate((float)deltaTime.getDeltaTime());
		renderer.render();

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
