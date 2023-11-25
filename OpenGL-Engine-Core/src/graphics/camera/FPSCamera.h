#pragma once

#include <graphics/camera/ICamera.h>
#include <graphics/Window.h>
#include <ui/DebugPane.h>



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
#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.1f
#define FPSCAMERA_MAX_FOV 90.0f

	class FPSCamera : public ICamera{
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
		
		FPSCamera(glm::vec3& position, glm::vec3& up, float yaw, float pitch);


		FPSCamera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch);



		virtual glm::mat4 getViewMatrix() override;

		virtual glm::mat4 getProjectionMatrix() override;

		void processInput(float deltaTime);

		// Getters
		inline float getYaw() const { return m_Yaw; }
		inline float getPitch() const { return m_Pitch; }
		inline float getMovementSpeed() const { return m_MovementSpeed; }
		inline float getMouseSensitivity() const { return m_MouseSensitivity; }
		inline float getFOV() const { return m_FOV; }

		inline virtual const glm::vec3& getPosition() const override{ return m_Position; }
		inline virtual const glm::vec3& getFront() const { return m_Front; }
		inline virtual const glm::vec3& getUp() const { return m_Up; }

		virtual void setPosition(const glm::vec3& position) override {
			m_Position = position;
		}
	private:

		void updateCameraVectors();


		void processKeyboard(Camera_Movement direction, float deltaTime);

		void processMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch);


		void processMouseScroll(double yOffset);
	};
} 