#include "pch.h"
#include "Light.h"

namespace OpenGL_Engine {  
	Light::Light(glm::vec3& lightColor)
		: m_LightColor(lightColor), m_IsStatic(false) {}

} 
