#pragma once


#include <utils/loaders/TextureLoader.h>

namespace OpenGL_Engine {
	class FrameBuffer {
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();

		void createFramebuffer();
		FrameBuffer& addTexture2DColorAttachment(bool multisampledBuffer);
		FrameBuffer& addDepthRBO(bool multisampledBuffer);
		FrameBuffer& addDepthStencilRBO(bool multisampledBuffer);
		FrameBuffer& addDepthAttachment(bool multisampledBuffer);

		void bind();
		void unbind();

		// Assumes framebuffer is bound
		void setColorAttachment(unsigned int target, unsigned int targetType, int mipLevel = 0);

		void clear();

		inline unsigned int getWidth() { return m_Width; }
		inline unsigned int getHeight() { return m_Height; }

		inline unsigned int getFramebuffer() { return m_FBO; }
		inline unsigned int getColourBufferTexture() { return m_ColourTexture; }
		inline unsigned int getDepthRBO() { return m_DepthRBO; }
		inline unsigned int getDepthStencilRBO() { return m_DepthStencilRBO; }
		inline unsigned int getDepthTexture() { return m_DepthTexture; }
		inline bool isMultisampledColourBuffer() { return m_IsMultiSampledColorBuffer; }
	private:
		unsigned int m_FBO;

		bool m_IsMultiSampledColorBuffer;
		// render targets(attachments)
		unsigned int m_ColourTexture;
		unsigned int m_DepthRBO;
		unsigned int m_DepthStencilRBO;
		unsigned int m_DepthTexture;
		unsigned int m_Width, m_Height;
	};
}