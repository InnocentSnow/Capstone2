#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "input.h"

class Keyboard : public Input
{

public:

	virtual float getX();
	virtual float getY();
	virtual float getZ();

private:

	Keyboard() {}
	virtual ~Keyboard() {}
	friend class Input;
};

#endif
