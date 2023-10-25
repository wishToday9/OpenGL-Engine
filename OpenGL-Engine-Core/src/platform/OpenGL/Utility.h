#pragma once

#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <stb_image.h>

namespace OpenGL_Engine { namespace opengl {

	class Utility {
		
	public:
		static GLuint loadTextureFromFile(const char *path);
	};

} }