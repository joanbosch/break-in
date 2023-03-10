#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

void Game::init()
{
	bPlay = true;
	state = MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	audio = new Audio();

	scene.init(1, 0, 0, 4, audio);
	instructions.init();
	credits.init();
	menu.init(audio);
	password.init();

}

bool Game::update(int deltaTime)
{
	switch (state) {
		case PLAY:
			scene.update(deltaTime);
			break;

		case INSTRUCTIONS:
			instructions.update(deltaTime);
			break;

		case CREDITS:
			credits.update(deltaTime);
			break;

		case MENU:
			menu.update(deltaTime);
			break;

		case PASSWORD:
			password.update(deltaTime);
			break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (state) {
	case PLAY:
		scene.render();
		break;

	case INSTRUCTIONS:
		instructions.render();
		break;

	case CREDITS:
		credits.render();
		break;

	case MENU:
		menu.render();
		break;

	case PASSWORD:
		password.render();
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		//bPlay = false;
		if (state == PLAY) {
			audio->stopAllSounds();
			audio->play(MENU_MUSIC, true);
		}
		state = MENU;
	}

	// KEYS TO CHANGE THE LEVEL!

	if (key == 49) { // '1' KEY: GO TO THE LEVEL1.
		state = PLAY;
		audio->stopAllSounds();
		audio->play(LEVEL1_MUSIC, true);
		scene.init(1, 0, 0, 4, audio);
	}
	if (key == 50) { // '2' KEY: GO TO THE LEVEL2.
		state = PLAY;
		audio->stopAllSounds();
		audio->play(LEVEL2_MUSIC, true);
		scene.init(2, 0, 0, 4, audio);
	}
	if (key ==51) { // '3' KEY: GO TO THE LEVEL3.
		state = PLAY;
		audio->stopAllSounds();
		audio->play(LEVEL3_MUSIC, true);
		scene.init(3, 0, 0, 4, audio);
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setState(int s)
{
	state = s;
}


void Game::setLvl(int l, int points, int money, int lives)
{
	switch (l) {
	case 1:
		audio->stopAllSounds();
		audio->play(LEVEL1_MUSIC, true);
		break;
	case 2:
		audio->stopAllSounds();
		audio->play(LEVEL2_MUSIC, true);
		break;
	case 3:
		audio->stopAllSounds();
		audio->play(LEVEL3_MUSIC, true);
		break;
	}
	scene.init(l, points, money, lives, audio);
}





