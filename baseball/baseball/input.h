#ifndef _INPUT_H
#define _INPUT_H

class Input
{

public:

	float getValue() { return getX(); }

	virtual float getX() = 0;
	virtual float getY() { return 0; }
	virtual float getZ() { return 0; }

	static Input *getInstance() { return &s_input; }

protected:

	static Input &s_input;
	Input() {}
	virtual ~Input() {}
};

#endif
