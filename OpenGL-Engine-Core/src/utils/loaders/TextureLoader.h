#pragma once
#include <map>
#include <stb_image.h>
#include "../../graphics/textures/Texture.h"

namespace OpenGL_Engine {
	namespace utils {
		class TextureLoader {
		private:
			struct DefaultTextures {
				graphics::Texture* m_DefaultDiffuse;
				graphics::Texture* m_FullSpecular, * m_NoSpecular;
				graphics::Texture* m_DefaultNormal;
				graphics::Texture* m_DefaultEmission;
			};

		public:
			static void InitializeDefaultTextures();
			static graphics::Texture* Load2DTexture(std::string& path);
			

			inline static graphics::Texture* getDefaultDiffuse() { return m_DefaultTextures.m_DefaultDiffuse; }
			inline static graphics::Texture* getDefaultSpecular() { return m_DefaultTextures.m_NoSpecular; }
			inline static graphics::Texture* getDefaultNormal() { return m_DefaultTextures.m_DefaultNormal; }
			inline static graphics::Texture* getDefaultEmission() { return m_DefaultTextures.m_DefaultEmission; }
			inline static graphics::Texture* getFullSpec() { return m_DefaultTextures.m_FullSpecular; }
			inline static graphics::Texture* getNoSpec() { return m_DefaultTextures.m_NoSpecular; }

		private:
			static std::map<std::string, graphics::Texture> m_TextureCache;
			static DefaultTextures m_DefaultTextures;
		};
	}
}