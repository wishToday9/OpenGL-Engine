#pragma once

#include "Defs.h"
namespace OpenGL_Engine { 

	struct TextureSettings {
		//texture format
		GLenum TextureFormat = GL_NONE; //if set to none, the data format will be used

		bool IsSRGB = false;

		//texture wrapping options
		GLenum TextureWrapSMode = GL_REPEAT;
		GLenum TextureWrapTMode = GL_REPEAT;
		bool HasBorder = false;
		glm::vec4 BorderColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//texture filtering options
		GLenum TextureMinificationFilterMode = GL_LINEAR_MIPMAP_LINEAR;// Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		GLenum TextureMagnificationFilterMode = GL_LINEAR; //// Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this sitation)

		float TextureAnisotropyLevel = ANISOTROPIC_FILTERING_LEVEL; // Specified independent of texture min and mag filtering, should be a power of 2 (1.0 means the usual isotropic texture filtering is used which means anisotropic filtering isn't used)

		// Mip options
		bool HasMips = true;
		int MipBias = 0; // positive means blurrier texture selected, negative means sharper texture which can show texture aliasing
	};


	class Texture {
	public:
		//default setting
		Texture(TextureSettings& settings = TextureSettings());

		~Texture();

		//generation functions
		void generate2DTexture(unsigned int width, unsigned int height, GLenum dataFormat, const void* data);	
		void generate2DMutisampleTexture(unsigned int width, unsigned int height);

		void bind(int uint = 0);
		void unbind();

		// Texture Tuning Functions (Works for pre-generation and post-generation)
		void setTextureWrapS(GLenum textureWrapMode);
		void setTextureWrapT(GLenum textureWrapMode);
		void setHasBorder(bool hasBorder);
		void setBorderColour(glm::vec4& borderColour);
		void setTextureMinFilter(GLenum textureFilterMode);
		void setTextureMagFilter(GLenum textureFilterMode);
		void setAnisotropicFilteringMode(float textureAnisotropyLevel);
		void setMipBias(int mipBias);

		// Pre-generation controls only
		void setHasMips(bool hasMips);
		void setTextureSettings(TextureSettings settings) {
			m_TextureSettings = settings;
		}

		unsigned int getTextureId() { return m_TextureId; }
		inline bool isGenerated() { return m_TextureId != 0; }
		unsigned int getWidth() { return m_Width; }
		unsigned int getHeight() { return m_Height; }
		const TextureSettings& getTextureSettings() { return m_TextureSettings; }
	private:
		// TODO: Need support for deletion/cleanup of cubemap
		unsigned int m_TextureId;
		GLenum m_TextureTarget;
		unsigned int m_Width, m_Height;

		TextureSettings m_TextureSettings;
	};
}