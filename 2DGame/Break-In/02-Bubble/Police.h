#ifndef _POLICE_INCLUDE
#define _POLICE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Police
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, int r);
	void update(int deltaTime, bool starMode);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return posPolice; };

	void setVelocity(float v);

	int getRoom() { return room; };
	bool isVisible() { return isOnScreen; };

	void setSearching();

	bool getCollisionPlayer() { return playerColided; };

private:
	glm::ivec2 tileMapDispl, posPolice, policeSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool playerColided, ballColided, isOnScreen, moving, visible;

	int room;
	float vel, currentTime, markTime;
	glm::vec2 goalPos;

	glm::vec2 N;
};


#endif // _PLAYER_INCLUDE

