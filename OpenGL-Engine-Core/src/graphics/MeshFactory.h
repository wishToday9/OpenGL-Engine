#pragma once


#include <vector>
#include "Mesh.h"
#include "../platform/OpenGL/Utility.h"

namespace OpenGL_Engine {
	namespace graphics {
		class MeshFactory {
		public:
			Mesh* CreateQuad(const char* path, bool shouldHaveSpec = false);

			// should be used to draw framebuffer's color buffer
			Mesh* CreateQuad(int colorBufferid);
		};
	}
}
