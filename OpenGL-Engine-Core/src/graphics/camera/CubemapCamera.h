#pragma once

#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Defs.h"

namespace OpenGL_Engine {  
	class CubemapCamera {
	public:
		CubemapCamera();
		CubemapCamera(glm::vec3& centerPosition);

		void swithCameraToFace(int cubeFaceIndex);
		void setCenterPosition(glm::vec3& newPosition) {
			m_CenterPosition = newPosition;
		}

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

	private:
		glm::vec3 m_CenterPosition, m_Front, m_Up;
	};
} 