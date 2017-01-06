#include <thread>
#include <gl/glut.h>
#include "game.h"
#include "core.h"
#include "text.h"
#include "texture.h"

void play()
{
	Game::getInstance()->play();
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	initGL(argc, argv);

	Text::getInstance()->init();
	Texture::getInstance()->init();
	std::thread t(&play);
	glutMainLoop();

	t.join();
	return 0;
}
