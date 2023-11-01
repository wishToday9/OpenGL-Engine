#include "Renderable3D.h"

namespace OpenGL_Engine { namespace graphics {

	Renderable3D::Renderable3D(glm::vec3& position, glm::vec3& scale, glm::vec3& rotationAxis, 
		float rotation, Model* model, Renderable3D* parent, bool shouldOutline, bool transparent)
		:m_Position(position), m_Scale(scale), m_Orientation(glm::angleAxis(rotation, rotationAxis)), m_Model(model),
		m_Parent(parent), m_ShouldOutline(shouldOutline), m_Transparent(transparent)
	{
	}

	Renderable3D::~Renderable3D() {
		
	}

	void Renderable3D::draw(Shader &shader) const {
		m_Model->Draw(shader);
	}
} }