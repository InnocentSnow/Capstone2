#include <gl/glew.h>
#include <gl/glut.h>
#include "texture.h"
#include "SOIL.h"

Texture Texture::s_instance;

GLuint loadTexture(const char *filename)
{
	return SOIL_load_OGL_texture(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

Texture::~Texture()
{
	for (TextureNameMap::iterator itor = m_map.begin(); itor != m_map.end(); ++itor)
	{
		glDeleteTextures(1, &itor->second);
	}
}

void Texture::init()
{
	m_map[Name::MAP] = loadTexture("data/map.png");
}

void Texture::useTexture(Name name)
{
	glColor3f(1, 1, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_map[name]);
}
