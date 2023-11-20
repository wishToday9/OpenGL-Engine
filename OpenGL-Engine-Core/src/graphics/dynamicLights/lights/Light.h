#pragma once


#include <graphics/Shader.h>

namespace OpenGL_Engine {  

	// TODO: Add functionality so it can update with an entitie's position and orientation
	class Light {
	public:
		Light(glm::vec3& lightColor);

		virtual void setupUniforms(Shader* shader, int currentLightIndex) = 0;
	protected:
		glm::vec3 m_LightColor;
		bool m_IsStatic; //should be true if the light will never have any of it's properties changed
	};

} 
