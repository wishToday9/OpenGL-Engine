#pragma once

namespace OpenGL_Engine {
	class ICamera {
	public:
		virtual ~ICamera() {}

		virtual glm::mat4 getViewMatrix() = 0;
		virtual glm::mat4 getProjectionMatrix() = 0;
		virtual const glm::vec3& getPosition() const = 0;
		virtual const glm::vec3& getFront() const = 0;
		virtual const glm::vec3& getUp() const = 0;
	};
}