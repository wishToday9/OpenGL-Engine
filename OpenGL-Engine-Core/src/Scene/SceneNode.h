#pragma once

#include <graphics/mesh/Model.h>
#include <graphics/renderer/RenderPass.h>

namespace OpenGL_Engine {
	namespace scene {
		class SceneNode {
		public:
			SceneNode(glm::vec3& position, glm::vec3& scale, glm::vec3& rotationAxis, float radianRotation, graphics::Model* model, SceneNode* parent, bool transparent = false);
			~SceneNode();
	// Assumes shader is already bound by the renderer

			void draw(graphics::Shader& shader, graphics::RenderPass pass) const;

			void addChild(SceneNode* child);



			inline const glm::vec3& getPosition() const { return m_Position; }

			inline const glm::vec3& getScale() const { return m_Scale; }

			inline const glm::quat& getOrientation() const { return m_Orientation; }

			inline const SceneNode* getParent() const { return m_Parent; }

			inline bool getTransparent() const { return m_Transparent; }

			inline void setPosition(glm::vec3& other) { m_Position = other; }

			inline void setScale(glm::vec3& other) { m_Scale = other; }

			inline void setOrientation(float radianRotation, glm::vec3 rotationAxis) { m_Orientation = glm::angleAxis(radianRotation, rotationAxis); }

			inline void setTransparent(bool choice) { m_Transparent = choice; }

			inline void setParent(SceneNode* parent) { m_Parent = parent; }

		private:

			glm::vec3 m_Position, m_Scale;

			glm::quat m_Orientation;

			SceneNode* m_Parent;

			std::vector<SceneNode*> m_Children;




			bool m_Transparent;

			graphics::Model* m_Model;

		};




	}
}