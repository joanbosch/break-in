#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Police.h"
#include "Game.h"

#define ESCALAT 2.f

#define VEL 1.5
#define ANIMATION_TIME 1300

enum PoliceAnims
{
	LOOK_FRONT, LOOK_RIGHT, LOOK_LEFT, DEAD, SEARCH
};


void Police::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, int r)
{
	policeSize = glm::vec2(32 * ESCALAT, 16 * ESCALAT);
	spritesheet.loadFromFile("images/police.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 32 * ESCALAT), glm::vec2(0.33, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(LOOK_FRONT, 8);
	sprite->addKeyframe(LOOK_FRONT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(LOOK_RIGHT, 8);
	sprite->addKeyframe(LOOK_RIGHT, glm::vec2(0.33f, 0.f));

	sprite->setAnimationSpeed(LOOK_LEFT, 8);
	sprite->addKeyframe(LOOK_LEFT, glm::vec2(0.66f, 0.f));

	sprite->setAnimationSpeed(SEARCH, 8);
	sprite->addKeyframe(SEARCH, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(SEARCH, glm::vec2(0.33f, 0.f));
	sprite->addKeyframe(SEARCH, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(SEARCH, glm::vec2(0.66f, 0.f));

	sprite->setAnimationSpeed(DEAD, 3);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.33f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.33f, 0.5f));

	sprite->changeAnimation(LOOK_FRONT);
	tileMapDispl = tileMapPos;
	posPolice = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));

	map = tileMap;
	room = r;
	vel = VEL;
	currentTime = 0.0f;
	markTime = ANIMATION_TIME;
	moving = false;
	isOnScreen = (room == map->getActualRoom());
	visible = true;
	ballColided = false;
}

void Police::update(int deltaTime, bool starMode)
{
	currentTime += deltaTime;

	if (visible) {
		isOnScreen = (room == map->getActualRoom());

		if (isOnScreen) {

			ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posPolice, glm::vec2(32 * ESCALAT, 32 * ESCALAT));
			if (starMode && ballColided) visible = false;

			// time to run animation
			if (currentTime >= markTime && !moving && !map->getScrolling()) {
				moving = true;
				goalPos = map->getPlayerPos();
				if (goalPos.x > posPolice.x) if (sprite->animation() != LOOK_RIGHT) sprite->changeAnimation(LOOK_RIGHT);
				if (goalPos.x < posPolice.x) if (sprite->animation() != LOOK_LEFT) sprite->changeAnimation(LOOK_LEFT);
				else {
					if (sprite->animation() != LOOK_FRONT) sprite->changeAnimation(LOOK_FRONT);
				}
			}
			if (moving) {
				if ((posPolice.x != goalPos.x) || (posPolice.y != goalPos.y)) {
					// tracking
					float Vx = goalPos.x - posPolice.x;
					float Vy = goalPos.y - posPolice.y;
					float length = sqrt(Vx * Vx + Vy * Vy);
					Vx *= vel / length;
					Vy *= vel / length;

					posPolice.x += Vx;
					posPolice.y += Vy;
				}
				else {
					markTime = currentTime + ANIMATION_TIME;
					if (sprite->animation() != SEARCH) sprite->changeAnimation(SEARCH);
					moving = false;
				}
			}
		}

		glm::vec2 posBar = posPolice;
		posBar.y += 28;

		playerColided = sprite->ballCollision(map->getPlayerPos(), glm::vec2(38 * ESCALAT, 64 * ESCALAT), posBar, policeSize);
		sprite->update(deltaTime);

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));
	}
}

void Police::render()
{
	if(visible) sprite->render();
}

void Police::setPosition(const glm::vec2& pos)
{
	posPolice = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));
}

void Police::setSearching() {
	if (sprite->animation() != SEARCH) sprite->changeAnimation(SEARCH);
}

void Police::setVelocity(float v) {
	vel = v;
}
