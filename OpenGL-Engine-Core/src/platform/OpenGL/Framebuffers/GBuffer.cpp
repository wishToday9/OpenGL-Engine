#include "pch.h"
#include "GBuffer.h"

namespace OpenGL_Engine {
	
	GBuffer::GBuffer(unsigned int width, unsigned int height)
		:FrameBuffer(width, height)
	{
		init();
	}

	GBuffer::~GBuffer()
	{
		for (size_t i = 0; i < m_GBufferRenderTargets.size(); ++i) {
			if (m_GBufferRenderTargets[i] != 0) {
				glDeleteTextures(1, &m_GBufferRenderTargets[i]);
			}
		}
	}

	void GBuffer::init()
	{
		addDepthAttachment(false);

		bind();

		//render target 1
		glGenTextures(1, &m_GBufferRenderTargets[0]);
		glBindTexture(GL_TEXTURE_2D, m_GBufferRenderTargets[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GBufferRenderTargets[0], 0);

		//render target 2
		glGenTextures(1, &m_GBufferRenderTargets[1]);
		glBindTexture(GL_TEXTURE_2D, m_GBufferRenderTargets[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GBufferRenderTargets[1], 0);

		//render target 3
		glGenTextures(1, &m_GBufferRenderTargets[2]);
		glBindTexture(GL_TEXTURE_2D, m_GBufferRenderTargets[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GBufferRenderTargets[2], 0);


		//finally tell OpenGL that we well be rendering to all of the attachments
		unsigned int attachments[3] = {
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2
		};
		glDrawBuffers(3, attachments);

		//check if the creation failed
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Logger::getInstance().error("logged_files/error.txt", "Framebuffer initialization", "Could not initialize GBuffer");
			return;
		}
		unbind();
	}

}