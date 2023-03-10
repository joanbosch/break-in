#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return posPlayer; };

	float getNewBallVelocity();
	bool getBallColided();
	float newVel(int dist);
	glm::vec2 getN();
	glm::vec2 computeNormalVector(glm::vec2 ballPos, glm::vec2 ballSize, glm::vec2 spritePosition, glm::vec2 spriteSize);
	float computeAngle(glm::vec2 ballPos);
	void updateAnimation(glm::vec2 ballPos, bool starMode);
	void setVisibility(bool vis);
	void setDead(bool b);

private:
	glm::ivec2 tileMapDispl, posPlayer, playerSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool visible, isDead;
	bool ballColided;

	float newBallVelocity;
	glm::vec2 N;
};


#endif // _PLAYER_INCLUDE