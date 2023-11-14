#pragma once

#include "Scene/SceneNode.h"
#include "Defs.h"
#include "graphics\camera\FPSCamera.h"
#include "graphics\renderer\MeshRenderer.h"
#include "graphics\renderer\GLCache.h"
#include "terrain\Terrain.h"
#include "graphics\Window.h"
#include "graphics\Skybox.h"
#include "graphics\DynamicLights\DynamicLightManager.h"
#include "utils\loaders\TextureLoader.h"

namespace OpenGL_Engine {
	
	class Scene3D {
	private:
		graphics::FPSCamera *m_Camera;
		graphics::MeshRenderer *m_MeshRenderer;
		terrain::Terrain *m_Terrain;
		graphics::Skybox *m_Skybox;
		graphics::DynamicLightManager m_DynamicLightManager;
		graphics::GLCache *m_GLCache;

		// Some sort of list of entities (tied to models that are in the Renderer (Renderable3D) (should this name be changed to Renderer3D?))
		//std::vector<Entity*> m_Entities;
		std::vector<scene::SceneNode*> m_Renderables;

		graphics::Shader m_TerrainShader, m_ModelShader, m_ShadowmapShader;
	public:
		Scene3D(graphics::FPSCamera *camera, graphics::Window *window);
		~Scene3D();
		
		void add(scene::SceneNode *renderable);
		
		//shadow pass
		void shadowmapPass();
		void onUpdate(float deltaTime);
		void onRender(unsigned int shadowmap);

		inline graphics::MeshRenderer* getRenderer() const { return m_MeshRenderer; }
		inline graphics::FPSCamera* getCamera() const { return m_Camera; }
	private:
		void init();
		void addObjectsToRenderQueue();
	};

}