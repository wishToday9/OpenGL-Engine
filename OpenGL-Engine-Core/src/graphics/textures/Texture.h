#pragma once

#include <glad/glad.h>
#include <glm/common.hpp>

#include "../../Defs.h"
#include "../../utils/Logger.h"

namespace OpenGL_Engine {namespace graphics {

	struct TextureSetting {
		//texture wrapping options
		GLenum TextureWrapSMode = GL_REPEAT;
		GLenum TextureWrapTMode = GL_REPEAT;

		//texture filtering options
		GLenum TextureMinFilterMode = GL_LINEAR_MIPMAP_LINEAR;// Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		GLenum TextureMagnificationFilterMode = GL_LINEAR; //// Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this sitation)

		float TextureAnisotropyLevel = ANISOTROPIC_FILTERING_LEVEL; // Specified independent of texture min and mag filtering, should be a power of 2 (1.0 means the usual isotropic texture filtering is used which means anisotropic filtering isn't used)

		// Mip options
		bool HasMips = true;
		int MipBias = 0; // positive means blurrier texture selected, negative means sharper texture which can show texture aliasing
	};


	class Texture {
	public:
		Texture();

		Texture(TextureSetting settings);
		~Texture();

		//generation functions
		void generate2DTexture(unsigned int width, unsigned int height, GLenum textureFormat, GLenum dataFormat, const void* data);	

		void bind(int uint = -1);
		void unbind();

		//textures tuning functions(Works for pre-generation and post-generation)
		void setTextureWrapS(GLenum textureWarpMode, bool shouldBind = false);
		void setTextureWrapT(GLenum textureWarpMode, bool shouldBind = false);
		void setTextureMinFilter(GLenum textureMinFilter, bool shouldBind = false);
		void setTextureMagFilter(GLenum textureMagFilter, bool shouldBind = false);
		void setAnisotropicFilteringMode(float textureAnisotropyLevel, bool shouldBind = false);

		// Pre-generation controls only
		void setMipMode(bool shouldGenMips, int mipBias);
		void setTextureSettings(TextureSetting settings) {
			m_TextureSettings = settings;
		}

		unsigned int getTextureId() { return m_TextureId; }
	private:
		// TODO: Need support for deletion/cleanup of cubemap
		unsigned int m_TextureId;
		GLenum m_TextureTarget;

		unsigned int m_Width, m_Height;
		GLenum m_TextureFormat;

		TextureSetting m_TextureSettings;
	};
}}