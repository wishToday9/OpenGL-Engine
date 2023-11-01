#pragma once

#include "graphics\camera\Camera.h"
#include "graphics\renderer\Renderer.h"

#include "graphics/renderer/Renderable3D.h"
#include "terrain/Terrain.h"
#include "graphics/Window.h"
#include "graphics/MeshFactory.h"
#include "graphics/Skybox.h"
#include "graphics/dynamicLights/DynamicLightManager.h"

namespace OpenGL_Engine {
	
	class Scene3D {
	private:
		graphics::Window* m_Window;
		graphics::Camera *m_Camera;
		graphics::Renderer *m_Renderer;
		terrain::Terrain* m_Terrain;
		graphics::Skybox* m_Skybox;
		graphics::DynamicLightManager m_DynamicLightManager;
		graphics::MeshFactory m_meshFactory;

		std::vector<graphics::Renderable3D*> m_Renderables;
		graphics::Shader m_TerrainShader, m_ModelShader, m_OutlineShader;


		// Some sort of list of entities (tied to models that are in the Renderer (should this be changed to Renderer3D?))
		//std::vector<Entity*> m_Entities;
	public:
		Scene3D(graphics::Camera* camera, graphics::Window* window);
		~Scene3D();

		void Add(graphics::Renderable3D* renderable);

		void onUpdate(float deltaTime);

		void onRender();

		inline graphics::Renderer* getRenderer() const { return m_Renderer; }
		inline graphics::Camera* getCamera() const { return m_Camera; }
	private:
		void init();
	};

}