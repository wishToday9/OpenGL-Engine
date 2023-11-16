#include "pch.h"
#include <scene/RenderableModel.h>

namespace OpenGL_Engine {

		RenderableModel::RenderableModel(glm::vec3& position, glm::vec3& scale, glm::vec3& rotationAxis, float radianRotation, Model* model, RenderableModel* parent, bool transparent)
			: m_Position(position), m_Scale(scale), m_Orientation(glm::angleAxis(radianRotation, rotationAxis)), m_Model(model), m_Parent(parent), m_IsTransparent(transparent)
		{

		}

		RenderableModel::~RenderableModel() {
			// Recursive delete on children
			for (unsigned i = 0; i < m_Children.size(); i++) {
				delete m_Children[i];
			}
		}

		void RenderableModel::draw(Shader* shader, RenderPassType pass) const {
			if (m_Model)
				m_Model->Draw(shader, pass);
		}

		void RenderableModel::addChild(RenderableModel* child) {
			if (child == this)
				return;

			m_Children.push_back(child);
			child->setParent(this);
		}
}