
#include "Timer.h"
#include <GLFW/glfw3.h>
namespace OpenGL_Engine {

	Timer::Timer() {
		startTime = glfwGetTime();
	}

	void Timer::reset() {
		startTime = glfwGetTime();
	}

};

