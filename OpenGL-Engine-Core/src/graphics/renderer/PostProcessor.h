#pragma once

#include <glad\glad.h>

namespace OpenGL_Engine { namespace graphics {

	class PostProcessor {
	public:
		PostProcessor();
	public:
		unsigned int texture;

		// Post Processing Toggles
		bool blur = false;
	};

} }