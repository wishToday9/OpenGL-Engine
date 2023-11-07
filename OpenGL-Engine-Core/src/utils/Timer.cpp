#include "Timer.h"

namespace OpenGL_Engine {

	Timer::Timer() {
		startTime = glfwGetTime();
	}

	void Timer::reset() {
		startTime = glfwGetTime();
	}

};

