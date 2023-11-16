#pragma once

#include <vector>
#include "../platform/OpenGL/VertexArray.h"
#include "../platform/OpenGL/IndexBuffer.h"
#include "../platform/OpenGL/Buffer.h"
#include "Shader.h"
#include "camera\FPSCamera.h"
#include "Window.h"
#include "../utils/loaders/TextureLoader.h"

#include <graphics/camera/ICamera.h>
#include <graphics/renderer/GLCache.h>

namespace OpenGL_Engine {  
	class Skybox {
	public:
		Skybox(const std::vector<std::string> &filePaths);

		void Draw(ICamera* camera);
		Cubemap* getSkyboxCubemap() { return m_SkyboxCubemap; }


	private:
		
		Shader* m_SkyboxShader;
		GLCache* m_GLCache;

		VertexArray m_SkyboxVAO;
		IndexBuffer m_SkyboxIBO;
		Buffer  m_SkyboxVBO;
		Cubemap* m_SkyboxCubemap; // Cubemap 
	};
} 