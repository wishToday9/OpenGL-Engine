#include "pch.h"

#include <graphics/Window.h>
#include <graphics/renderer/MasterRenderer.h>
#include <scene/Scene3D.h>
#include <ui/DebugPane.h>
#include <ui/RuntimePane.h>
#include <ui/WaterPane.h>
#include <utils/Time.h>



int main() {
	// Prepare the engine
	OpenGL_Engine::Window window("OpenGL_Engine Engine", WINDOW_X_RESOLUTION, WINDOW_Y_RESOLUTION);
	OpenGL_Engine::TextureLoader::initializeDefaultTextures();
	OpenGL_Engine::Scene3D scene(&window);
	OpenGL_Engine::MasterRenderer renderer(&scene);
	OpenGL_Engine::InputManager manager;


	// Prepare the UI
	OpenGL_Engine::RuntimePane runtimePane(glm::vec2(270.0f, 175.0f));
	OpenGL_Engine::DebugPane debugPane(glm::vec2(270.0f, 230.0f));
	OpenGL_Engine::WaterPane waterPane(glm::vec2(270.0f, 400.0f));

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

		OpenGL_Engine::Window::bind();
		OpenGL_Engine::Window::clear();

		ImGui_ImplGlfwGL3_NewFrame();

		//manager.Update();

		scene.onUpdate((float)deltaTime.getDeltaTime());
		renderer.render();

		// Display panes
		if (!OpenGL_Engine::Window::getHideUI())
		{
			OpenGL_Engine::Window::bind();
			runtimePane.render();
			debugPane.render();
			waterPane.render();
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();

	}
	return 0;
}
