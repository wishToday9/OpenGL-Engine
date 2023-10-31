#pragma once

#include "../Model.h"
#include <glm\glm.hpp>

namespace OpenGL_Engine { namespace graphics {

	class Renderable3D {
	public:
		Renderable3D(glm::vec3 &position, glm::vec3& scale, glm::vec3& rotationAxis, float rotation,
			Model *model, bool shouldOutline = false, bool transparent = false);
		~Renderable3D();

		void draw(Shader &shader) const;

		const glm::vec3 getPosition() const { return m_Position; }
		const glm::vec3 getScale() const { return m_Scale; }
		const glm::vec3 getRotationAxis() const { return m_RotationAxis; }
		float getRadianRotation() const { return m_Rotation; }
		bool getShouldOutline() const { return m_ShouldOutline; }
		bool getTransparent() const { return m_Transparent; }

		void setPosition(glm::vec3& other) { m_Position = other; }
		void setScale(glm::vec3& other) { m_Scale = other; }
		void setRotationAxis(glm::vec3& other) { m_RotationAxis = other; }
		void setRadianTotation(float value) { m_Rotation = value; }
		void setShouldOutline(bool choice) { m_ShouldOutline = choice; }
		void setTransparent(bool choice) { m_Transparent = choice; }

	private:
		glm::vec3 m_Position, m_Scale, m_RotationAxis;
		float m_Rotation;
		bool m_ShouldOutline, m_Transparent;

		Model *m_Model;
	};

} }