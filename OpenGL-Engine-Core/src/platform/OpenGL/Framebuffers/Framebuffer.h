#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "../../../utils/Logger.h"
#include "../../../Defs.h"


namespace OpenGL_Engine {
	namespace opengl {

		class Framebuffer {
		public:
			Framebuffer(int width, int height);
			~Framebuffer();

			void createFramebuffer();
			Framebuffer& addColorAttachment(bool multiSampledBuffer);
			Framebuffer& addDepthStencilRBO(bool multiSampledBuffer);

			void bind();
			void unbind();

			inline GLuint getFramebuffer() { return m_FBO; }
			inline GLuint getColourBufferTexture() { return m_ColourTexture; }
		private:
			GLuint m_FBO, m_DepthStencilRBO, m_ColourTexture;

			bool m_Created;
			unsigned int m_Width, m_Height;
		};

	}
}