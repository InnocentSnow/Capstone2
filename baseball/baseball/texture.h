#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <map>

class Texture
{

public:

	enum Name
	{
		MAP
	};

private:

	typedef std::map<Name, unsigned int> TextureNameMap;
	TextureNameMap m_map;

	Texture() {}
	static Texture s_instance;

public:

	~Texture();

	void init();
	void useTexture(Name name);

	static Texture *getInstance() { return &s_instance; }
};

#endif
