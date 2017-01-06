#include <gl/glew.h>
#include <gl/glut.h>
#include <string>
#include <fstream>
#include "HopShaderMgr.h"
#include "HopError.h"

#include "hop.sl"

enum
{
	BUFSIZE = 1024
};

HopShaderMgr HopShaderMgr::s_instance;

HopShaderMgr::HopShaderMgr() : m_hopProgram(0)
{

}

HopShaderMgr::~HopShaderMgr()
{
	if (m_hopProgram) glDeleteProgram(m_hopProgram);
}

HopShaderMgr *HopShaderMgr::getInstance()
{
	return &s_instance;
}

static GLuint createShader(const char *src, GLenum type)
{
	GLuint shader = glCreateShader(type);

	if (!shader)
	{
		hopError("Cannot Create Shader\n");
		return 0;
	}

	GLint isCompiled;
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		GLsizei len;
		GLchar error[BUFSIZE];

		glGetShaderInfoLog(shader, BUFSIZE, &len, error);
		hopError("Failed to Compile Shader\n%s\n", error);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

class ShaderSafeReleaser
{
public:

	ShaderSafeReleaser(GLuint vs, GLuint fs, GLuint program) : m_vs(vs), m_fs(fs), m_program(program) {}
	~ShaderSafeReleaser()
	{
		if (m_program) glDeleteProgram(m_program);
		if (m_vs) glDeleteShader(m_vs);
		if (m_fs) glDeleteShader(m_fs);
	}

	void done()
	{
		m_program = 0;
	}

private:
	GLuint m_vs;
	GLuint m_fs;
	GLuint m_program;
};

static GLuint createProgram(const char *vertexSrc, const char *fragmentSrc)
{
	GLuint vertexShader = createShader(vertexSrc, GL_VERTEX_SHADER);
	GLuint fragmentShader = createShader(fragmentSrc, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	ShaderSafeReleaser r(vertexShader, fragmentShader, program);

	if (!vertexShader || !fragmentShader || !program)
	{
		hopError("Failed to create program : ");
		hopError("vertex - \n[%s], fragment - \n[%s]\n", vertexSrc, fragmentSrc);

		return 0;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

	if (!isLinked)
	{
		hopError("Failed to link program\n");

		GLsizei len;
		GLchar error[BUFSIZE];
		glGetProgramInfoLog(program, BUFSIZE, &len, error);
		hopError("%s\n", error);

		return 0;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	r.done();
	return program;
}

void HopShaderMgr::load()
{
	glewInit();
	m_hopProgram = createProgram(VertexShader, FragmentShader);

	if (!m_hopProgram)
	{
		hopError("Cannot create hop program\n");
		abort();
	}
}

void HopShaderMgr::useHopProgram()
{
	if (!m_hopProgram) load();
	glUseProgram(m_hopProgram);
}


int HopShaderMgr::getPositionAttrib()
{
	if (!m_hopProgram) load();
	return glGetAttribLocation(m_hopProgram, "a_position");
}

int HopShaderMgr::getTexCoordAttrib()
{
	if (!m_hopProgram) load();
	return glGetAttribLocation(m_hopProgram, "a_texCoord");
}

int HopShaderMgr::getTextureUniform()
{
	if (!m_hopProgram) load();
	return glGetUniformLocation(m_hopProgram, "u_position");
}