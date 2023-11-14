#pragma once

#include <glad/glad.h>
#include "Defs.h"
namespace OpenGL_Engine { namespace graphics {
	struct CubemapSettings
	{
		//texture wrapping options
//texture wrapping options
		GLenum TextureWrapSMode = GL_CLAMP_TO_EDGE;
		GLenum TextureWrapTMode = GL_CLAMP_TO_EDGE;
		GLenum TextureWrapRMode = GL_CLAMP_TO_EDGE;


		//texture filtering options
		GLenum TextureMinificationFilterMode = GL_LINEAR;// Filtering mode when the texture moves further away and multiple texels map to one pixel (trilinear for best quality)
		GLenum TextureMagnificationFilterMode = GL_LINEAR; //// Filtering mode when the texture gets closer and multiple pixels map to a single texel (Never needs to be more than bilinear because that is as accurate as it gets in this sitation)

		float TextureAnisotropyLevel = ANISOTROPIC_FILTERING_LEVEL; // Specified independent of texture min and mag filtering, should be a power of 2 (1.0 means the usual isotropic texture filtering is used which means anisotropic filtering isn't used)
	};



	class Cubemap {
	public:
		Cubemap();
		Cubemap(CubemapSettings settings);
		~Cubemap();

		void generateCubemapFace(GLenum face, unsigned int faceWidth, unsigned int faceHeight,
			GLenum textureFormat, GLenum dataFormat, const unsigned char* data);

		void bind(int unit = -1);
		void unbind();

		//pre-generation controls only
		void setCubemapSettings(CubemapSettings settings) {
			m_CubemapSettings = settings;
		}
	private:
	
		unsigned int m_CubemapId;

		unsigned int m_FaceWidth, m_FaceHeight;
		GLenum m_TextureFormat;

		CubemapSettings m_CubemapSettings;
	};
}}