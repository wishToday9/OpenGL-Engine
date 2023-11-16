#include "Scene3D.h"

#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/common/Cube.h"
#include "graphics/mesh/common/Sphere.h"
#include "graphics/mesh/common/Quad.h"
#include <graphics/mesh/Model.h>
#include <Scene/RenderableModel.h>

namespace OpenGL_Engine {

	Scene3D::Scene3D(Window* window)
		: m_SceneCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f), 
		m_DynamicLightManager(), m_ModelRenderer(getCamera()), m_Terrain(glm::vec3(0.0f, -20.0f, 0.0f))
	{
		m_GLCache = GLCache::getInstance();

		init();
	}

	Scene3D::~Scene3D() {

	}

	void Scene3D::init() {
		m_GLCache->setMultisample(true);

		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(90.0f, 60.0f, 90.0f), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0, new arcane::Model("res/3D_Models/Crysis/nanosuit.obj"), nullptr, false
		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(200.0f, 50.0f, 100.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(0.0f), new arcane::Model("res/3D_Models/Sponza/sponza.obj"), nullptr, false));

		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(40, 60, 40), glm::vec3(15, 15, 15), glm::vec3(0.0, 1.0, 0.0), glm::radians(180.0f), glass, nullptr, true));
		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(80, 60, 80), glm::vec3(15, 15, 15), glm::vec3(0.0, 1.0, 0.0), glm::radians(180.0f), glass, nullptr, true));
		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(120, 60, 120), glm::vec3(15, 15, 15), glm::vec3(0.0, 1.0, 0.0), glm::radians(180.0f), glass, nullptr, true));

		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(20, 90, 20), glm::vec3(10, 10, 10), glm::vec3(1, 0, 0), 0, new Model(Cube()), nullptr, false));
		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(140, 90, 140), glm::vec3(10, 10, 10), glm::vec3(1, 0, 0), 0, new Model(Sphere()), nullptr, false));
		//m_RenderableModels.push_back(new Renderable3D(glm::vec3(-20, 90, -20), glm::vec3(10, 10, 10), glm::vec3(1, 0, 0), 0, new Model(Quad()), nullptr, false));

		// Temp code until I rewrite the model loader
		Model* pbrGun = new OpenGL_Engine::Model("res/3D_Models/Cerberus_Gun/Cerberus_LP.FBX");
		m_RenderableModels.push_back(new RenderableModel(glm::vec3(120.0f, 120.0f, 120.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-90.0f), pbrGun, nullptr, false));
		pbrGun->getMeshes()[0].getMaterial().setAlbedoMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_A.tga"), true));
		pbrGun->getMeshes()[0].getMaterial().setNormalMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_N.tga"), false));
		pbrGun->getMeshes()[0].getMaterial().setMetallicMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_M.tga"), false));
		pbrGun->getMeshes()[0].getMaterial().setRoughnessMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_R.tga"), false));
		pbrGun->getMeshes()[0].getMaterial().setAmbientOcclusionMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Cerberus_Gun/Textures/Cerberus_AO.tga"), false));


		// Temp testing code
		
		int nrRows = 2;
		int nrColumns = 2;
		float spacing = 2.5;
		for (int row = 0; row < nrRows; row++) {
			for (int col = 0; col < nrColumns; col++) {
				Model* sphere = new OpenGL_Engine::Model("res/3D_Models/Sphere/globe-sphere.obj");
				Material& mat = sphere->getMeshes()[0].getMaterial();
				mat.setAlbedoMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_basecolor.png"), true));
				mat.setNormalMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_normal.png"), false));
				mat.setAmbientOcclusionMap(TextureLoader::load2DTexture(std::string("res/textures/default/white.png"), false));
				mat.setMetallicMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_metallic.png"), false));
				mat.setRoughnessMap(TextureLoader::load2DTexture(std::string("res/3D_Models/Sphere/rustediron2_roughness.png"), false));
				m_RenderableModels.push_back(new RenderableModel(glm::vec3((float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, sphere, nullptr, false));
			}
		}

		// Skybox
		std::vector<std::string> skyboxFilePaths;
		skyboxFilePaths.push_back("res/skybox/right.png");
		skyboxFilePaths.push_back("res/skybox/left.png");
		skyboxFilePaths.push_back("res/skybox/top.png");
		skyboxFilePaths.push_back("res/skybox/bottom.png");
		skyboxFilePaths.push_back("res/skybox/back.png");
		skyboxFilePaths.push_back("res/skybox/front.png");
		m_Skybox = new Skybox(skyboxFilePaths);
	}


	float rotAmount = 0.0f;
	void Scene3D::onUpdate(float deltaTime) {
		m_RenderableModels[0]->setOrientation(rotAmount, glm::vec3(0.0f, 1.0f, 0.0f));
		rotAmount += deltaTime;
		m_SceneCamera.processInput(deltaTime);

		m_DynamicLightManager.setSpotLightDirection(m_SceneCamera.getFront());
		m_DynamicLightManager.setSpotLightPosition(m_SceneCamera.getPosition());
	}

	void Scene3D::onRender() {

	}

	void Scene3D::addModelsToRenderer() {
		auto iter = m_RenderableModels.begin();
		while (iter != m_RenderableModels.end()) {
			RenderableModel* curr = *iter;
			if (curr->getTransparent()) {
				m_ModelRenderer.submitTransparent(curr);
			}
			else {
				m_ModelRenderer.submitOpaque(curr);
			}
			iter++;
		}
	}

}
