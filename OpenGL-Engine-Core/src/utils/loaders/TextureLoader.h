#pragma once
#include <map>
#include <stb_image.h>
#include "../../graphics/textures/Texture.h"
#include "../../graphics/textures/Cubemap.h"

#include <string>


namespace OpenGL_Engine {
	namespace utils {
		class TextureLoader {
		private:
			struct DefaultTextures {
				graphics::Texture* m_DefaultAlbedo;
				graphics::Texture* m_DefaultNormal;
				graphics::Texture* m_FullMetallic, * m_NoMetallic;
				graphics::Texture* m_FullRoughness, * m_NoRoughness;
				graphics::Texture* m_DefaultAO;
				graphics::Texture* m_DefaultEmission;
			};

		public:
			static void initializeDefaultTextures();
			static graphics::Texture* load2DTexture(std::string& path, bool isSRGB, graphics::TextureSetting* settings = nullptr);

			static graphics::Cubemap* loadCubemapTexture(const std::string& right, const std::string& left, 
				const std::string& top, const std::string& bottom, const std::string& back, const std::string& front, 
				bool isSRGB, graphics::CubemapSettings* settings = nullptr);
			


			inline static graphics::Texture* getDefaultAlbedo() { return m_DefaultTextures.m_DefaultAlbedo; }
			inline static graphics::Texture* getDefaultNormal() { return m_DefaultTextures.m_DefaultNormal; }
			inline static graphics::Texture* getDefaultMetallic() { return m_DefaultTextures.m_NoMetallic; }
			inline static graphics::Texture* getDefaultRoughness() { return m_DefaultTextures.m_NoRoughness; }
			inline static graphics::Texture* getDefaultAO() { return m_DefaultTextures.m_DefaultAO; }
			inline static graphics::Texture* getDefaultEmission() { return m_DefaultTextures.m_DefaultEmission; }
			inline static graphics::Texture* getFullMetallic() { return m_DefaultTextures.m_FullMetallic; }
			inline static graphics::Texture* getNoMetallic() { return m_DefaultTextures.m_NoMetallic; }
			inline static graphics::Texture* getFullRoughness() { return m_DefaultTextures.m_FullRoughness; }
			inline static graphics::Texture* getNoRoughness() { return m_DefaultTextures.m_NoRoughness; }

		private:
			static std::map<std::string, graphics::Texture> m_TextureCache;
			static DefaultTextures m_DefaultTextures;
		};
	}
}