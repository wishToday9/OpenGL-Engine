#pragma once

#include <platform/OpenGL/Framebuffers/FrameBuffer.h>

namespace OpenGL_Engine {
	class GBuffer :public FrameBuffer {
	public:
		GBuffer(unsigned int width, unsigned int height);
		~GBuffer();
		unsigned int getAlbedo() { return m_GBufferRenderTargets[0]; }
		unsigned int getNormal() { return m_GBufferRenderTargets[1]; }
		unsigned int getMaterialInfo() { return m_GBufferRenderTargets[2]; }

	private:
		void init();
	private:
		//0 RGBA8  -> albedo.r albedo.g albedo.b  albedo.a
		//1 RGB16F -> normal.x normal.y normal.y
		//2 RGBA8  -> metallic roughness ambientOcclusion
		std::array<unsigned int, 3> m_GBufferRenderTargets;

	};
}