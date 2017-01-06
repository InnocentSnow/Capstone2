#include <gl/glut.h>
#include "text.h"

Text Text::s_instance;

void Text::init()
{
	for (char i = ' '; i <= '~'; ++i) m_char[i].init(i);
}

void Text::draw(const char *str, float x, float y, float size)
{
	for (int i = 0; str[i]; ++i)
	{
		if (m_char[str[i]].isInitialized())
		{
			glPushMatrix();
			glTranslatef(x, y, 0);
			
			m_char[str[i]].applyTexture();

			glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3f(0, 0, 0);
			glTexCoord2f(1, 1);
			glVertex3f(size, 0, 0);
			glTexCoord2f(1, 0);
			glVertex3f(size, size, 0);
			glTexCoord2f(0, 0);
			glVertex3f(0, size, 0);
			glEnd();

			glPopMatrix();

			x += size;
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}
