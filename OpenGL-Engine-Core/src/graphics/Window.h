#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <input/InputManager.h>

namespace OpenGL_Engine {

	static InputManager g_InputManager;

	class Window {
	public:

		Window(const char* title, int width, int height);
		~Window();


		GLFWwindow* getNativeWindow() const {
			return m_Window;
		}

		void update();
		static void clear();
		static void bind();

		bool closed() const;

		static bool isKeyPressed(unsigned int keycode);
		static bool isMouseButtonPressed(unsigned int code);


		static inline bool getHideCursor() { return s_HideCursor; }
		static inline bool getHideUI() { return s_HideUI; }
		static inline int getWidth() { return s_Width; }
		static inline int getHeight() { return s_Height; }
		static inline int getRenderResolutionWidth() { return (int)(s_Width * SUPERSAMPLING_FACTOR); }
		static inline int getRenderResolutionHeight() { return (int)(s_Height * SUPERSAMPLING_FACTOR); }
	private:
		bool init();

		void setFullscreenResolution();

		// Callback Functions
		static friend void error_callback(int error, const char* description);
		static friend void window_resize_callback(GLFWwindow* window, int width, int height);
		static friend void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
		static friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static friend void char_callback(GLFWwindow* window, unsigned int c);
		static friend void joystick_callback(int joystick, int event);
		static friend void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id,
			GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	private:
		const char* m_Title;
		GLFWwindow* m_Window;
		static bool s_HideCursor;
		static bool s_HideUI;
		static int s_Width, s_Height;
	};

}