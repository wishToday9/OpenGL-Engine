#pragma once

#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <stb_image.h>
#include <vector>
#include <glm\common.hpp>
#include "../../utils/Logger.h"
#include "../../Defs.h"

namespace OpenGL_Engine { namespace opengl {

	class Utility {
		
	public:
		static GLuint loadTextureFromFile(const char *path, bool containsTransparencyOnSides = false);
		static GLuint loadCubemapFromFile(const std::vector<const char*>& vec);
	};

} }