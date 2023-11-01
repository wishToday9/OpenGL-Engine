#pragma once

#include "../Model.h"
#include <glm\glm.hpp>

namespace OpenGL_Engine { namespace graphics {

	class Renderable3D {
	public:
		Renderable3D(glm::vec3 &position, glm::vec3& scale, glm::vec3& rotationAxis, float rotation,
			Model *model, Renderable3D* parent, bool shouldOutline = false, bool transparent = false);
		~Renderable3D();

		void draw(Shader &shader) const;

		const glm::vec3 getPosition() const { return m_Position; }
		const glm::vec3 getScale() const { return m_Scale; }
		const glm::quat& getOrientation() const { return m_Orientation; }
		const Renderable3D* getParent() const { return m_Parent; }
		bool getShouldOutline() const { return m_ShouldOutline; }
		bool getTransparent() const { return m_Transparent; }

		void setPosition(glm::vec3& other) { m_Position = other; }
		void setScale(glm::vec3& other) { m_Scale = other; }
		void setShouldOutline(bool choice) { m_ShouldOutline = choice; }
		void setTransparent(bool choice) { m_Transparent = choice; }

	private:
		glm::vec3 m_Position, m_Scale;
		glm::quat m_Orientation;
		Renderable3D* m_Parent;


		bool m_ShouldOutline, m_Transparent;

		Model *m_Model;
	};

} }