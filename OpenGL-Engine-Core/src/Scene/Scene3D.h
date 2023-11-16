#pragma once

#include "Scene/RenderableModel.h"
#include "Defs.h"
#include "graphics\camera\FPSCamera.h"
#include "graphics\renderer\ModelRenderer.h"
#include "graphics\renderer\GLCache.h"
#include "terrain\Terrain.h"
#include "graphics\Window.h"
#include "graphics\Skybox.h"
#include "graphics\DynamicLights\DynamicLightManager.h"
#include "utils\loaders\TextureLoader.h"

#include <graphics/IBL/EnvironmentProbe.h>


namespace OpenGL_Engine {
	class Scene3D {
	public:
		Scene3D(Window *window);
		~Scene3D();

		void onUpdate(float deltaTime);
		void onRender();


		void addModelsToRenderer();

		inline ModelRenderer* getModelRenderer() { return &m_ModelRenderer; }
		inline Terrain* getTerrain() { return &m_Terrain; }
		inline DynamicLightManager* getDynamicLightManager() { return &m_DynamicLightManager; }
		inline FPSCamera* getCamera() { return &m_SceneCamera; }
		inline Skybox* getSkybox() { return m_Skybox; }
	private:
		void init();
		void addObjectsToRenderQueue();
	private:
		// Global Data
		GLCache* m_GLCache;

		// Scene Specific Data
		FPSCamera m_SceneCamera;
		Skybox* m_Skybox;
		ModelRenderer m_ModelRenderer;
		Terrain m_Terrain;
		DynamicLightManager m_DynamicLightManager;
		std::vector<EnvironmentProbe*> m_Probes;
		std::vector<RenderableModel*> m_RenderableModels;

	};

}