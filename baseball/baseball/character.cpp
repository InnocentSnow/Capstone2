#include <gl/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#include "character.h"

Character::Character() : m_texture(0), m_isInit(false)
{
}

Character::~Character()
{
	if (m_texture) glDeleteTextures(1, &m_texture);
}

void Character::init(char c)
{
	unsigned int frameBuffer, depthBuffer;

	m_isInit = true;

	glGenFramebuffers(1, &frameBuffer);
	glGenRenderbuffers(1, &depthBuffer);
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE || !frameBuffer || !depthBuffer || !m_texture)
	{
		printf("Failed to Create Render Target\n");
		return;
	}

	glViewport(0, 0, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glRasterPos2f(-1, -1);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);

	float p[4];
	glGetFloatv(GL_CURRENT_RASTER_POSITION, p);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	glRasterPos2f(-(p[0] / RENDER_TARGET_WIDTH) * 0.5f, -0.5f);

	glDepthFunc(GL_ALWAYS);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	glDepthFunc(GL_LESS);

	glGetFloatv(GL_CURRENT_RASTER_POSITION, p);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glViewport(0, 0, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);

	if (frameBuffer) glDeleteFramebuffers(1, &frameBuffer);
	if (depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);
}

void Character::applyTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
