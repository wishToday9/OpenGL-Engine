#include "pch.h"
#include "Light.h"

namespace OpenGL_Engine {  
	Light::Light(float intensity, glm::vec3& lightColor)
		: m_Intensity(intensity), m_LightColor(lightColor), m_IsStatic(false) {}

} 
