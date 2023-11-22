#pragma once
#include <utils/loaders/TextureLoader.h>


namespace OpenGL_Engine {
	
	enum ColorAttachmentFormat
	{
		Normalized8 = GL_RGBA8,
		Nomralized16 = GL_RGBA16,
		FloatingPoint16 = GL_RGBA16F,
		FloatingPoint32 = GL_RGBA32F
	};

	enum DepthStencilAttachmentFormat
	{
		NormalizedDepthOnly = GL_DEPTH_COMPONENT,
		NormalizedDepthStencil = GL_DEPTH24_STENCIL8,
		FloatingPointDepthStencil = GL_DEPTH32F_STENCIL8
	};


	class Framebuffer {
	public:
		Framebuffer(unsigned int width, unsigned int height, bool isMultisampled);
		virtual ~Framebuffer();

		void createFramebuffer();
		Framebuffer& addColorTexture(ColorAttachmentFormat textureFormat);
		Framebuffer& addDepthStencilTexture(DepthStencilAttachmentFormat textureFormat);
		Framebuffer& addDepthStencilRBO(DepthStencilAttachmentFormat rboFormat);

		void bind();
		void unbind();

		// Assumes framebuffer is bound
		void setColorAttachment(unsigned int target, unsigned int targetType, int mipLevel = 0);

		void clear();

		// Getters
		inline bool isMultisampled() const { return m_IsMultisampled; }
		inline unsigned int getFramebuffer() { return m_FBO; }
		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height; }
		inline Texture* getColourTexture() { return &m_ColourTexture; }
		inline Texture* getDepthStencilTexture() { return &m_DepthStencilTexture; }
		inline unsigned int getDepthStencilRBO() { return m_DepthStencilRBO; }

	protected:
		unsigned int m_FBO;

		unsigned int m_Width, m_Height;
		bool m_IsMultisampled;


		// render targets(attachments)
		Texture m_ColourTexture;
		Texture m_DepthStencilTexture;
		unsigned int m_DepthStencilRBO;
	};
}