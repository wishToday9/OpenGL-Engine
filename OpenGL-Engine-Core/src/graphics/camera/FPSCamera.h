#pragma once

#include <graphics/Window.h>
#include <ui/DebugPane.h>

#include <glm/common.hpp>


namespace OpenGL_Engine {  

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UPWARDS,
		DOWNWARDS
	};

	// Default Camera Values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 10.0f;
	const float SENSITIVITY = 0.10f;
	const float FOV = 100.0f;

	class FPSCamera {
	private:
		// Camera Attributes
		glm::vec3 m_Position, m_Front, m_Up, m_Right, m_WorldUp;

		// Euler Angles
		float m_Yaw;
		float m_Pitch;

		// Camera Options
		float m_MovementSpeed;
		float m_MouseSensitivity;
		float m_FOV;
	public:
		
		FPSCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);


		FPSCamera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch);


		/**
		* Returns the view matrix for the camera's position, yaw, and pitch
		*
		* @return returns the view matrix for the camera
		*/
		glm::mat4 getViewMatrix();

		glm::mat4 getProjectionMatrix();

		void processInput(float deltaTime);

		// Getters
		inline float getYaw() const { return m_Yaw; }
		inline float getPitch() const { return m_Pitch; }
		inline float getMovementSpeed() const { return m_MovementSpeed; }
		inline float getMouseSensitivity() const { return m_MouseSensitivity; }
		inline float getFOV() const { return m_FOV; }
		inline const glm::vec3& getFront() const { return m_Front; }
		inline const glm::vec3& getPosition() const { return m_Position; }
	private:

		void updateCameraVectors();


		void processKeyboard(Camera_Movement direction, float deltaTime);

		void processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch);


		void processMouseScroll(double yOffset);
	};
} 