
#pragma once

#define NUM_JOYSTICK_BUTTONS 16

class JoystickInputData
{
public:
	// Range [-1, 1]
	float m_LeftStickHorizontal;
	float m_LeftStickVertical;
	float m_RightStickHorizontal;
	float m_RightStickVertical;

	// Range [0, 1] R1 and l1
	float m_LeftTrigger;
	float m_RightTrigger;


	// Store the state of each joytick button to be queried
	bool m_ButtonStates[NUM_JOYSTICK_BUTTONS];

	JoystickInputData();
	~JoystickInputData();

	inline void SetConnection(bool isConnected) { m_Connected = isConnected; }
	inline bool IsConnected() { return m_Connected; }


private:
	bool m_Connected;
};