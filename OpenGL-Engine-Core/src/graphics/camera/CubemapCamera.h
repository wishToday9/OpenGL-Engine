#pragma once

#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera/ICamera.h>

#include "../../Defs.h"

namespace OpenGL_Engine {  
	class CubemapCamera : public ICamera{
	public:
		CubemapCamera();
		CubemapCamera(glm::vec3& centerPosition);

		void switchCameraToFace(int cubeFaceIndex);
		void setCenterPosition(glm::vec3& newPosition) {
			m_CenterPosition = newPosition;
		}

		virtual glm::mat4 getViewMatrix() override;
		virtual glm::mat4 getProjectionMatrix() override;
		inline virtual const glm::vec3& getPosition() const override { return m_CenterPosition; }
		inline virtual const glm::vec3& getFront() const { return m_Front; }
		inline virtual const glm::vec3& getUp() const { return m_Up; }
	private:
		glm::vec3 m_CenterPosition, m_Front, m_Up;
	};
} 