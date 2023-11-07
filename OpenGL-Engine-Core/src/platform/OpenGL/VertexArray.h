#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Buffer.h"
#include <vector>

namespace OpenGL_Engine { namespace opengl {

	class VertexArray {
	private:
		unsigned int m_VertexArrayID;
		std::vector<Buffer*> m_Buffers;
	public:
		VertexArray();
		~VertexArray();

		// Function for automatically adding non-interleaved buffer data
		void addBuffer(Buffer *buffer, int index);

		void bind() const;
		void unbind() const;
	};

} }