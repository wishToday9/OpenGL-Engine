
#include <GLFW/glfw3.h>

#include "Time.h"

namespace OpenGL_Engine {

	Time::Time() {
		lastFrame = glfwGetTime();
		delta = 0;
	}

	void Time::update() {
		delta = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
	}

}