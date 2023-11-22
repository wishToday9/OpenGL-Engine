#include "pch.h"
#include "Framebuffer.h"

namespace OpenGL_Engine {

	Framebuffer::Framebuffer(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height), m_FBO(0), m_IsMultiSampledColorBuffer(false), m_ColourTexture(0), m_DepthTexture(0), m_DepthRBO(0), m_DepthStencilRBO(0)
	{
		glGenFramebuffers(1, &m_FBO);
	}

	Framebuffer::~Framebuffer() {
		if (m_ColourTexture != 0) {
			glDeleteTextures(1, &m_ColourTexture);
		}
		if (m_DepthTexture != 0) {
			glDeleteTextures(1, &m_DepthTexture);
		}
		if (m_DepthRBO != 0) {
			glDeleteRenderbuffers(1, &m_DepthRBO);
		}
		if (m_DepthStencilRBO != 0) {
			glDeleteRenderbuffers(1, &m_DepthStencilRBO);
		}

		glDeleteFramebuffers(1, &m_FBO);
	}

	void Framebuffer::createFramebuffer() {
		bind();
		if (m_ColourTexture == 0) {
			// Indicate that there won't be a colour buffer for this FBO
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		// Check if the creation failed
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Could not initialize the framebuffer");
			return;
		}
		unbind();
	}

	Framebuffer& Framebuffer::addTexture2DColorAttachment(bool multisampledBuffer) {
		m_IsMultiSampledColorBuffer = multisampledBuffer;

#if DEBUG_ENABLED
		if (m_ColourTexture != 0) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Framebuffer already has a colour attachment");
			return *this;
		}
#endif

		bind();
		glGenTextures(1, &m_ColourTexture);

		// Generate colour texture attachment
		if (multisampledBuffer) {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColourTexture);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, GL_RGBA16F, m_Width, m_Height, GL_TRUE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColourTexture, 0);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, m_ColourTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourTexture, 0);
		}

		unbind();
		return *this;
	}

	Framebuffer& Framebuffer::addDepthRBO(bool multisampledBuffer) {
#if DEBUG_ENABLED
		if (m_DepthRBO != 0) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Framebuffer already has a depth RBO attachment");
			return *this;
		}
#endif

		bind();

		// Generate the depth rbo attachment
		glGenRenderbuffers(1, &m_DepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
		if (multisampledBuffer)
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLE_AMOUNT, GL_DEPTH_COMPONENT32, m_Width, m_Height);
		else
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_Width, m_Height);

		// Attach the depth rbo attachment
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

		unbind();
		return *this;
	}

	Framebuffer& Framebuffer::addDepthStencilRBO(bool multisampledBuffer) {
#if DEBUG_ENABLED
		if (m_DepthStencilRBO != 0) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Framebuffer already has a depth+stencil RBO attachment");
			return *this;
		}
#endif

		bind();

		// Generate depth+stencil rbo attachment
		glGenRenderbuffers(1, &m_DepthStencilRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRBO);
		if (multisampledBuffer) {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLE_AMOUNT, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		}
		else {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		}

		// Attach depth+stencil attachment
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRBO);

		unbind();
		return *this;
	}

	Framebuffer& Framebuffer::addDepthAttachment(bool multisampledBuffer) {
#if DEBUG_ENABLED
		if (m_DepthTexture != 0) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Framebuffer already has a depth attachment");
			return *this;
		}
#endif

		bind();

		// Generate depth attachment
		glGenTextures(1, &m_DepthTexture);

		if (multisampledBuffer) {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, GL_DEPTH_COMPONENT32, m_Width, m_Height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture, 0);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
		}

		unbind();
		return *this;
	}

	Framebuffer& Framebuffer::addDepthStencilAttachment(bool multisampledBuffer) {
#if DEBUG_ENABLED
		if (m_DepthTexture != 0) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Framebuffer already has a depth attachment");
			return *this;
		}
#endif

		bind();

		// Generate depth attachment
		glGenTextures(1, &m_DepthTexture);

		if (multisampledBuffer) {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLE_AMOUNT, GL_DEPTH24_STENCIL8, m_Width, m_Height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthTexture, 0);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
		}

		unbind();
		return *this;
	}

	void Framebuffer::setColorAttachment(unsigned int target, unsigned int targetType, int mipToWriteTo) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targetType, target, mipToWriteTo);
	}

	void Framebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	void Framebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}