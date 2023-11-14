#pragma once

#include "Scene/RenderableModel.h"
#include "Defs.h"
#include "graphics\camera\FPSCamera.h"
#include "graphics\renderer\MeshRenderer.h"
#include "graphics\renderer\GLCache.h"
#include "terrain\Terrain.h"
#include "graphics\Window.h"
#include "graphics\Skybox.h"
#include "graphics\DynamicLights\DynamicLightManager.h"
#include "utils\loaders\TextureLoader.h"

#include <graphics/IBL/EnvironmentProbeManager.h>

namespace OpenGL_Engine {
	class Scene3D {
	private:
		GLCache *m_GLCache;
		FPSCamera *m_Camera;
		MeshRenderer *m_MeshRenderer;
		Terrain *m_Terrain;
		Skybox *m_Skybox;
		DynamicLightManager m_DynamicLightManager;
		EnvironmentProbeManager m_ProbeManager;

		// Some sort of list of entities (tied to models that are in the Renderer (Renderable3D) (should this name be changed to Renderer3D?))
		//std::vector<Entity*> m_Entities;
		std::vector<RenderableModel*> m_Renderables;

		Shader m_TerrainShader, m_ModelShader, m_ShadowmapShader;
	public:
		Scene3D(FPSCamera *camera, Window *window);
		~Scene3D();
		
		void add(RenderableModel *renderable);
		
		//shadow pass
		void shadowmapPass();
		void onUpdate(float deltaTime);
		void onRender(unsigned int shadowmap);

		inline MeshRenderer* getRenderer() const { return m_MeshRenderer; }
		inline FPSCamera* getCamera() const { return m_Camera; }
	private:
		void init();
		void addObjectsToRenderQueue();
	};

}