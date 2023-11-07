#pragma once

#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <glm\common.hpp>
#include "../../utils/Logger.h"
#include "../../Defs.h"

namespace OpenGL_Engine { namespace opengl {

	class Utility {
	public:
		//static unsigned int loadTextureFromFile(const char *path, bool containsTransparencyOnSides = false);
		static unsigned int loadCubemapFromFiles(const std::vector<const char*> &vec);
	};

} }