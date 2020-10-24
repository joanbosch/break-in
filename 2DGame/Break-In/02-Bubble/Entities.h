#ifndef ENTITIES_INCLUDE
#define ENTITIES_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Block.h"
#include "Axe.h"
#include "Wood.h"
#include "Coin.h"
#include "Bag.h"

class Entities
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map);
	void update(int deltaTime);
	void render();

	bool ballHasColided();
	glm::vec2 getN();

private:
	glm::ivec2 tileMapDispl, posPlayer;

	bool ballColided;
	glm::vec2 N;

	vector<Block*> *blocks;
	vector<Wood*> *woods;
	vector<Coin*> *single_coins;
	vector<Bag*> *bags;
	/* TODO: implement these sprites too
	vector<MultipleCoins*> *multiple_coins;
	vector<Diamond*> *diamonds;
	Alarm* alarm;
	*/
	Axe *axe;

	TileMap *map;
	// vector<pair<int,int>*> blocks_desp, wood_desp, coin_desp, bag_desp;
};


#endif // ENTITIES_INCLUDE


