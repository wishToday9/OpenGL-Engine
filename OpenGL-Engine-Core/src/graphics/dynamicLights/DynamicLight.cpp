#include "DynamicLight.h"

namespace OpenGL_Engine {
	namespace graphics {
		DynamicLight::DynamicLight(glm::vec3& amb, glm::vec3& dif, glm::vec3& spec)
			:ambient(amb), diffuse(dif), specular(spec), isActive(false) {}
	}
	
}