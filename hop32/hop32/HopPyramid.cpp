#include <gl/glew.h>
#include <gl/glut.h>
#include <math.h>
#include "HopPyramid.h"
#include "HopShaderMgr.h"
#include "HopRenderTarget.h"

enum
{
	NUM_RENDER_TARGET = 4
};

HopPyramid::HopPyramid() : m_vao(0)
{

}

HopPyramid::~HopPyramid()
{
	if (m_vao) glDeleteBuffers(1, &m_vao);
}

struct vertex
{
	vertex(float _px, float _py, float _tx, float _ty) : px(_px), py(_py), tx(_tx), ty(_ty) { }

	float px, py;
	float tx, ty;
};

void HopPyramid::init()
{
	float t = 0.02f;
	vertex vertices[] = {
		vertex(-1, 1, 1, 0), vertex(-1, -1, 0, 0), vertex(0, 0, 0.5f, 1),
		vertex(1, -1, 1, 0), vertex(1, 1, 0, 0), vertex(0, 0, 0.5f, 1),
		vertex(-1, -1, 1, 0), vertex(1, -1, 0, 0), vertex(0, 0, 0.5f, 1),
		vertex(1, 1, 1, 0), vertex(-1, 1, 0, 0), vertex(0, 0, 0.5f, 1),

		vertex(1 + t, 1 - t, 0, 0), vertex(1 - t, 1 + t, 0, 0), vertex(-1 - t, -1 + t, 0, 0), vertex(-1 + t, -1 - t, 0, 0),
		vertex(-1 + t, 1 + t, 0, 0), vertex(-1 - t, 1 - t, 0, 0), vertex(1 - t, -1 - t, 0, 0), vertex(1 + t, -1 + t, 0, 0)
	};

	glGenBuffers(1, &m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	HopShaderMgr *shaderMgr = HopShaderMgr::getInstance();
	m_attrPosition = shaderMgr->getPositionAttrib();
	m_attrTexCoord = shaderMgr->getTexCoordAttrib();
	m_uniformTexture = shaderMgr->getTextureUniform();
}

void HopPyramid::draw(HopRenderTarget renderTarget[])
{
	int savedFace, savedCullMode;
	glGetIntegerv(GL_FRONT_FACE, &savedFace);
	glGetIntegerv(GL_CULL_FACE, &savedCullMode);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	HopShaderMgr::getInstance()->useHopProgram();

	glBindBuffer(GL_ARRAY_BUFFER, m_vao);

	glEnableVertexAttribArray(m_attrPosition);
	glVertexAttribPointer(m_attrPosition, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);

	glEnableVertexAttribArray(m_attrTexCoord);
	glVertexAttribPointer(m_attrTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(2 * sizeof(float)));

	glUniform1i(m_uniformTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < 2; ++i)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 3 * NUM_RENDER_TARGET + 4 * i, 4);
	}

	for (int i = 0; i < NUM_RENDER_TARGET; ++i)
	{
		renderTarget[i].applyTexture();
		glDrawArrays(GL_TRIANGLES, 3 * i, 3);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < 2; ++i)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 3 * NUM_RENDER_TARGET + 4 * i, 4);
	}

	glUseProgram(0);

	glFrontFace(savedFace);
	glCullFace(savedCullMode);
}
