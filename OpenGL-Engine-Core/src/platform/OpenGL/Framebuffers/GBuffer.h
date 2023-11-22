#pragma once

#include <platform/OpenGL/Framebuffers/Framebuffer.h>
#include <graphics/textures/Texture.h>

namespace OpenGL_Engine {
	class GBuffer :public Framebuffer {
	public:
		GBuffer(unsigned int width, unsigned int height);
		~GBuffer();
		inline Texture* getAlbedo() { return &m_GBufferRenderTargets[0]; }
		inline Texture* getNormal() { return &m_GBufferRenderTargets[1]; }
		inline Texture* getMaterialInfo() { return &m_GBufferRenderTargets[2]; }

	private:
		void init();
	private:
		//0 RGBA8  -> albedo.r albedo.g albedo.b  albedo.a
		//1 RGB16F -> normal.x normal.y normal.y
		//2 RGBA8  -> metallic roughness ambientOcclusion
		std::array<Texture, 3> m_GBufferRenderTargets;

	};
}