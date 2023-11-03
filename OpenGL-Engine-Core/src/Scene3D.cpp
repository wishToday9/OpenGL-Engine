#include "Scene3D.h"
#include <iterator>
#include <iostream>
#include <glm/glm.hpp>

namespace OpenGL_Engine {
	
	Scene3D::Scene3D(graphics::Camera* camera, graphics::Window* window)
		: m_TerrainShader("src/shaders/basic.vert", "src/shaders/terrain.frag"),
		m_ModelShader("src/shaders/basic.vert", "src/shaders/model.frag"),
		m_OutlineShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
		m_Camera(camera), m_Window(window), m_DynamicLightManager()
	{
		m_Renderer = new graphics::Renderer(camera);
		m_Terrain = new terrain::Terrain(glm::vec3(0.0f, -20.0f, 0.0f));

		init();
	}

	Scene3D::~Scene3D()
	{

	}

	void Scene3D::Add(graphics::Renderable3D* renderable)
	{
		m_Renderables.push_back(renderable);

	}

	void Scene3D::onUpdate(float deltaTime)
	{
		//m_Renderables[0]->setRadianTotation(m_Renderables[0]->getRadianRotation() + deltaTime);
	}

	void Scene3D::onRender()
	{

		m_DynamicLightManager.setSpotLightDirection(m_Camera->getFront());
		m_DynamicLightManager.setSpotLightPosition((m_Camera->getPosition()));

		//setup
		m_OutlineShader.enable();
		m_OutlineShader.setUniformMat4("view", m_Camera->getViewMatrix());
		m_OutlineShader.setUniformMat4("projection", glm::perspective(glm::radians(m_Camera->getFOV()), 
			(float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 1000.0f));
		

		//models
		m_ModelShader.enable();
		m_DynamicLightManager.setupLightingUniforms(m_ModelShader);
		m_ModelShader.setUniform3f("viewPos", m_Camera->getPosition());
		m_ModelShader.setUniformMat4("view", m_Camera->getViewMatrix());
		m_ModelShader.setUniformMat4("projection", glm::perspective(glm::radians(m_Camera->getFOV()), 
			(float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 1000.0f));

		std::vector<graphics::Renderable3D*>::iterator iter = m_Renderables.begin();

		while (iter != m_Renderables.end()) {
			graphics::Renderable3D* curr = *iter;
			if (curr->getTransparent()) {
				m_Renderer->submitTransparent(curr);
			}
			else {
				m_Renderer->submitOpaque(curr);
			}
			iter++;
		}


		m_Renderer->flushOpaque(m_ModelShader, m_OutlineShader);

		// terrain
		glStencilMask(0x00);
		m_TerrainShader.enable();
		m_DynamicLightManager.setupLightingUniforms(m_TerrainShader);
		m_TerrainShader.setUniform3f("viewPos", m_Camera->getPosition());
		glm::mat4 modelMatrix(1);
		modelMatrix = glm::translate(modelMatrix, m_Terrain->getPosition());
		m_TerrainShader.setUniformMat4("model", modelMatrix);
		m_TerrainShader.setUniformMat4("view", m_Camera->getViewMatrix());
		m_TerrainShader.setUniformMat4("projection", glm::perspective(glm::radians(m_Camera->getFOV()),
			(float)m_Window->getWidth() / (float)m_Window->getHeight(), 0.1f, 1000.0f));
		m_Terrain->Draw(m_TerrainShader);

		m_Skybox->Draw();

		//transparent objects
		m_ModelShader.enable();
		m_Renderer->flushTransparent(m_ModelShader, m_OutlineShader);
	
	}

	void Scene3D::init()
	{
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);


		//load models
		std::vector<graphics::Mesh> meshes;

		meshes.push_back(*m_meshFactory.CreateQuad("res/textures/window.png", true));

		std::vector<graphics::Mesh> meshes2;
		meshes2.push_back(*m_meshFactory.CreateQuad("res/textures/grass.png", true));


		Add(new graphics::Renderable3D(glm::vec3(30.0f, -10.0f, 30.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::radians(90.f),
			new OpenGL_Engine::graphics::Model("res/3D_Models/Overwatch/Reaper/Reaper.FBX"), nullptr, true));
		Add(new graphics::Renderable3D(glm::vec3(60.0f, -10.0f, 60.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::radians(90.f),
			new OpenGL_Engine::graphics::Model("res/3D_Models/Overwatch/McCree/McCree.FBX"), nullptr, false));
		//Add(new graphics::Renderable3D(glm::vec3(90.0f, -10.0f, 90.0f), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0, new arcane::graphics::Model("res/3D_Models/Crysis/nanosuit.obj"), true));
		//Add(new graphics::Renderable3D(glm::vec3(200.0f, 200.0f, 100.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(0.0f), new arcane::graphics::Model("res/3D_Models/Sponza/sponza.obj")));
		Add(new graphics::Renderable3D(glm::vec3(40, 20, 40), glm::vec3(15, 15, 15), glm::vec3(1.0, 0.0, 0.0),
			glm::radians(90.0f), new graphics::Model(meshes), nullptr, false, true));
		Add(new graphics::Renderable3D(glm::vec3(80, 20, 80), glm::vec3(15, 15, 15), glm::vec3(1.0, 0.0, 0.0),
			glm::radians(90.0f), new graphics::Model(meshes), nullptr, false, true));
		Add(new graphics::Renderable3D(glm::vec3(120, 20, 120), glm::vec3(15, 15, 15), glm::vec3(1.0, 0.0, 0.0), 
			glm::radians(90.0f), new graphics::Model(meshes), nullptr, false, true));


		// Terrain shader
		m_TerrainShader.enable();
		m_TerrainShader.setUniform1f("material.shininess", 128.0f);


		//Model Shader
		m_ModelShader.enable();
		m_ModelShader.setUniform1f("material.shininess", 128.0f);

		//skybox
		std::vector<const char*> skyboxFilePaths;
		skyboxFilePaths.push_back("res/skybox/right.png");
		skyboxFilePaths.push_back("res/skybox/left.png");
		skyboxFilePaths.push_back("res/skybox/top.png");
		skyboxFilePaths.push_back("res/skybox/bottom.png");
		skyboxFilePaths.push_back("res/skybox/back.png");
		skyboxFilePaths.push_back("res/skybox/front.png");
		m_Skybox = new graphics::Skybox(skyboxFilePaths, m_Camera, m_Window);

	}

}