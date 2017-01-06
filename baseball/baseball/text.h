#ifndef _TEXT_H
#define _TEXT_H

#include "character.h"

class Text
{

private:

	Character m_char[128];
	static Text s_instance;

public:

	void init();
	void draw(const char *str, float x, float y, float size);

	static Text *getInstance() { return &s_instance; }
};

#endif

