#include "pch.h"

#include <graphics/Window.h>
#include <graphics/renderer/MasterRenderer.h>
#include <scene/Scene3D.h>
#include <ui/DebugPane.h>
#include <ui/RuntimePane.h>
#include <utils/Time.h>



int main() {
	// Prepare the engine
	OpenGL_Engine::Window window("OpenGL_Engine Engine", WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
	OpenGL_Engine::TextureLoader::initializeDefaultTextures();
	OpenGL_Engine::Scene3D scene(&window);
	OpenGL_Engine::MasterRenderer renderer(&scene);
	OpenGL_Engine::InputManager manager;


	// Prepare the UI
	OpenGL_Engine::RuntimePane runtimePane(glm::vec2(256.0f, 90.0f));
	OpenGL_Engine::DebugPane debugPane(glm::vec2(256.0f, 115.0f));

	// Initialize the renderer
	renderer.init();

	OpenGL_Engine::Time deltaTime;
	while (!window.closed()) {

		if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window.getNativeWindow(), true);

		deltaTime.update();

#if DEBUG_ENABLED
		if (debugPane.getWireframeMode())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

		window.clear();
		ImGui_ImplGlfwGL3_NewFrame();

		//manager.Update();

		scene.onUpdate((float)deltaTime.getDeltaTime());
		renderer.render();

		// Display panes
		runtimePane.render();
		debugPane.render();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();

	}
	return 0;
}
