#pragma once


#include <graphics/Shader.h>

namespace OpenGL_Engine {  

	// TODO: Add functionality so it can update with an entitie's position and orientation
	struct DynamicLight {
		DynamicLight(glm::vec3& lightColor);

		virtual void setupUniforms(Shader &shader, int currentLightIndex) = 0;


		glm::vec3 lightColor;
		bool isActive;
	};

} 
