#include "pch.h"

#include "FPSCamera.h"
#include <graphics/Window.h>

namespace OpenGL_Engine {	 

	FPSCamera::FPSCamera(glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_CurrentMovementSpeed(FPSCAMERA_MAX_SPEED), m_CurrentMouseSensitivity(FPSCAMERA_ROTATION_SENSITIVITY), m_CurrentFOV(FPSCAMERA_MAX_FOV)
	{
		m_Position = position;
		m_WorldUp = up;
		m_Up = up;
		m_CurrentYaw = yaw;
		m_CurrentPitch = pitch;
		updateCameraVectors();

		DebugPane::bindCameraPositionValue(&m_Position);
	}

	FPSCamera::FPSCamera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw = -90.0f, float pitch = 0.0f)
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_CurrentMovementSpeed(FPSCAMERA_MAX_SPEED), m_CurrentFOV(FPSCAMERA_MAX_FOV)
	{
		m_Position = glm::vec3(xPos, yPos, zPos);
		m_WorldUp = glm::vec3(xUp, yUp, zUp);
		m_CurrentYaw = yaw;
		m_CurrentPitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 FPSCamera::getViewMatrix() {
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	glm::mat4 FPSCamera::getProjectionMatrix() {
		return glm::perspective(glm::radians(m_CurrentFOV), (float)Window::getWidth() / (float)Window::getHeight(), NEAR_PLANE, FAR_PLANE);
	}

	void FPSCamera::processInput(float deltaTime) {
		// Keyboard input
		if (InputManager::isKeyPressed(GLFW_KEY_W))
			processKeyboard(OpenGL_Engine::FORWARD, deltaTime);
		if (InputManager::isKeyPressed(GLFW_KEY_S))
			processKeyboard(OpenGL_Engine::BACKWARD, deltaTime);
		if (InputManager::isKeyPressed(GLFW_KEY_A))
			processKeyboard(OpenGL_Engine::LEFT, deltaTime);
		if (InputManager::isKeyPressed(GLFW_KEY_D))
			processKeyboard(OpenGL_Engine::RIGHT, deltaTime);
		if (InputManager::isKeyPressed(GLFW_KEY_SPACE))
			processKeyboard(OpenGL_Engine::UPWARDS, deltaTime);
		if (InputManager::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			processKeyboard(OpenGL_Engine::DOWNWARDS, deltaTime);
		if (InputManager::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
			m_CurrentMovementSpeed = FPSCAMERA_MAX_SPEED * 4.0f;
		else if (InputManager::isKeyPressed(GLFW_KEY_LEFT_ALT))
			m_CurrentMovementSpeed = FPSCAMERA_MAX_SPEED / 4.0f;
		else
			m_CurrentMovementSpeed = FPSCAMERA_MAX_SPEED;

		// Mouse scrolling
		processMouseScroll(InputManager::getScrollYDelta() * 6.0);

		// Mouse movement
		processMouseMovement(InputManager::getMouseXDelta(), -InputManager::getMouseYDelta(), true);
	}

	void FPSCamera::invertPitch()
	{
		m_CurrentPitch = -m_CurrentPitch;
		updateCameraVectors();
	}

	void FPSCamera::processKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = m_CurrentMovementSpeed * deltaTime;
		switch (direction) {
		case FORWARD:
			m_Position += m_Front * velocity;
			break;
		case BACKWARD:
			m_Position -= m_Front * velocity;
			break;
		case LEFT:
			m_Position -= m_Right * velocity;
			break;
		case RIGHT:
			m_Position += m_Right * velocity;
			break;
		case UPWARDS:
			m_Position += m_WorldUp * velocity;
			break;
		case DOWNWARDS:
			m_Position -= m_WorldUp * velocity;
			break;
		}
	}

	void FPSCamera::processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true) {

		// Make sure the user isn't interacting with the UI
		if (!Window::getHideCursor())
			return;

		xOffset *= m_CurrentMouseSensitivity;
		yOffset *= m_CurrentMouseSensitivity;

		m_CurrentYaw += (float)xOffset;
		m_CurrentPitch += (float)yOffset;

		// Constrain the pitch
		if (constrainPitch) {
			if (m_CurrentPitch > 89.0f) {
				m_CurrentPitch = 89.0f;
			}
			else if (m_CurrentPitch < -89.0f) {
				m_CurrentPitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	void FPSCamera::processMouseScroll(double offset) {
		if (offset != 0 && m_CurrentFOV >= 1.0 && m_CurrentFOV <= FPSCAMERA_MAX_FOV) {
			m_CurrentFOV -= (float)offset;
		}
		if (m_CurrentFOV < 1.0f) {
			m_CurrentFOV = 1.0f;
		}
		else if (m_CurrentFOV > FPSCAMERA_MAX_FOV) {
			m_CurrentFOV = FPSCAMERA_MAX_FOV;
		}
	}

	void FPSCamera::updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(m_CurrentYaw)) * cos(glm::radians(m_CurrentPitch));
		front.y = sin(glm::radians(m_CurrentPitch));
		front.z = sin(glm::radians(m_CurrentYaw)) * cos(glm::radians(m_CurrentPitch));
		m_Front = glm::normalize(front);

		// Recalculate Vectors
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

} 