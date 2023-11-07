#pragma once

#include <glad\glad.h>

namespace arcane { namespace graphic {

	class PostProcessor {
	public:
		PostProcessor();
	public:
		unsigned int texture;

		// Post Processing Toggles
		bool blur = false;
	};

} }