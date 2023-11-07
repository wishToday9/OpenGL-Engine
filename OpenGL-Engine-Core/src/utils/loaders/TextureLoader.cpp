#include "TextureLoader.h"

namespace OpenGL_Engine { namespace utils {
		
	
	void TextureLoader::InitializeDefaultTextures()
	{
		m_DefaultTextures.m_DefaultDiffuse = Load2DTexture(std::string("res/textures/default/defaultDiffuse.png"));
		m_DefaultTextures.m_FullSpecular = Load2DTexture(std::string("res/textures/default/fullSpec.png"));
		m_DefaultTextures.m_NoSpecular = Load2DTexture(std::string("res/textures/default/noSpec.png"));
		m_DefaultTextures.m_DefaultNormal = Load2DTexture(std::string("res/textures/default/defaultNormal.png"));
		m_DefaultTextures.m_DefaultEmission = Load2DTexture(std::string("res/textures/default/defaultEmission.png"));
	}

	OpenGL_Engine::graphics::Texture* TextureLoader::Load2DTexture(std::string& path)
	{
		//check the cache
		std::map<std::string, graphics::Texture>::iterator iter = m_TextureCache.find(path);
		if (iter != m_TextureCache.end()) {
			return &iter->second;
		}

		//load the textures
		int width, height, numComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &numComponents, 0);
		if (!data) {
			utils::Logger::getInstance().error("logged_files/texture_loading.txt", "texture load fail - path:", path);
			stbi_image_free(data);
		graphics::Texture texture;
			return nullptr;
		}

		GLenum format;
		switch (numComponents) {
		case 1: format = GL_RED;  break;
		case 3: format = GL_RGB;  break;
		case 4: format = GL_RGBA; break;
		}

		graphics::Texture texture;
		texture.Generate2DTexture(width, height, format, format, data);

		// Add the texture to the cache (unless the user wants full control over the texture they loaded)

		m_TextureCache.insert(std::pair<std::string, graphics::Texture>(path, texture));
		

		return &m_TextureCache[path];
	}

	std::map<std::string, OpenGL_Engine::graphics::Texture> TextureLoader::m_TextureCache;

	TextureLoader::DefaultTextures TextureLoader::m_DefaultTextures;

}}