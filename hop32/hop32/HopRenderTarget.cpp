#include <gl/glew.h>
#include <gl/glut.h>
#include "HopRenderTarget.h"
#include "HopError.h"

#ifndef NULL
#define NULL 0
#endif

HopRenderTarget::HopRenderTarget() : m_frameBuffer(0), m_depthBuffer(0), m_texture(0)
{

}

HopRenderTarget::~HopRenderTarget()
{
	if (m_frameBuffer) glDeleteFramebuffers(1, &m_frameBuffer);
	if (m_depthBuffer) glDeleteRenderbuffers(1, &m_depthBuffer);
	if (m_texture) glDeleteTextures(1, &m_texture);
}

void HopRenderTarget::init()
{
	glGenFramebuffers(1, &m_frameBuffer);
	glGenRenderbuffers(1, &m_depthBuffer);
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE || !m_frameBuffer || !m_depthBuffer || !m_texture)
	{
		hopError("Failed to Create Render Target\n");
		return;
	}

	applyRenderTarget();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cancelRenderTarget(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
}

void HopRenderTarget::applyTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
void HopRenderTarget::applyRenderTarget()
{
	glViewport(0, 0, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
}

void HopRenderTarget::cancelRenderTarget(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	int size = width < height ? width : height;
	glViewport((width - size) / 2, (height - size) / 2, size, size);
}
