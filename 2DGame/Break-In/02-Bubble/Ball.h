#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Audio.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Audio *audio);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::vec2 getPosition();
	void treatCollision(glm::vec2 N);
	void setNewDirection(glm::vec2 ref);
	void setVisibility(bool vis);
	float getVelocity();
	float getAngle();
	void setVelocity(float v);
	void setGameStarted(bool s);
	void setStarMode(bool s);

private:
	glm::ivec2 tileMapDispl, posPlayer;
	float x_pos, y_pos;

	float angle, vel; // Vel -> pixels/second, angle-> angle that the ball is going on.

	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool visible, gameStarted, starMode;

	Audio* audioManager;
};


#endif // _BALL_INCLUDE


