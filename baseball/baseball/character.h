#ifndef _CHARACTER_H
#define _CHARACTER_H

class Character
{

private:

	enum
	{
		RENDER_TARGET_WIDTH = 32,
		RENDER_TARGET_HEIGHT = 32
	};

	unsigned int m_texture;
	bool m_isInit;

public:

	Character();
	~Character();

	bool isInitialized() { return m_isInit; }
	void init(char c);
	void applyTexture();
};

#endif
