#include <glm/gtc/matrix_transform.hpp>
#include <gl/glut.h>
#include "Hop32.h"

HopSide::HopSide(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ)
{
	m_eye[0] = eyeX;
	m_eye[1] = eyeY;
	m_eye[2] = eyeZ;

	m_at[0] = atX;
	m_at[1] = atY;
	m_at[2] = atZ;
}

static glm::mat4 getGlmViewMatrix(float eye[], float at[])
{
	return glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]), glm::vec3(at[0], at[1], at[2]), glm::vec3(0, 1, 0));
}

static glm::mat4 getGlmProjectionMatrix()
{
	static const float EPSILON = 0.1f;
	static const float FOV_Y = glm::radians(90.0f);
	static const float ASPECT = 2.0f;

	float size = Hop::getSize() / 2;
	float zNear = EPSILON < EPSILON * size ? EPSILON : size;

	glm::mat4 mat = glm::perspective(FOV_Y, ASPECT, zNear, zNear + 1);

	mat[2][2] = -1;
	mat[3][2] = -2 * zNear;

	return mat;
}

static glm::mat4 getGlmViewProjectionMatrix(float eye[], float at[])
{
	return getGlmProjectionMatrix() * getGlmViewMatrix(eye, at);
}

static void copyGlmMatrix(const glm::mat4 &mat, float out[])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			out[i * 4 + j] = mat[i][j];
		}
	}
}

void HopSide::getViewMatrix(float out[])
{
	copyGlmMatrix(getGlmViewMatrix(m_eye, m_at), out);
}

void HopSide::getProjectionMatrix(float out[])
{
	copyGlmMatrix(getGlmProjectionMatrix(), out);
}

void HopSide::getViewProjectionMatrix(float out[])
{
	copyGlmMatrix(getGlmViewProjectionMatrix(m_eye, m_at), out);
}

void HopSide::gluPerspective()
{
	float mat[16];
	getProjectionMatrix(mat);
	glMultMatrixf(mat);
}

void HopSide::gluLookAt()
{
	float mat[16];
	getViewMatrix(mat);
	glMultMatrixf(mat);
}
