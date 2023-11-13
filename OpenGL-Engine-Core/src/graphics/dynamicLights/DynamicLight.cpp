#include "DynamicLight.h"

namespace OpenGL_Engine { namespace graphics {

	DynamicLight::DynamicLight(glm::vec3& lightColor)
		: lightColor(lightColor), isActive(false) {}

} }
