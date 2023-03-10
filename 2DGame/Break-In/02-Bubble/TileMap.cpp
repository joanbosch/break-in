#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	setPlayableArea(1 * tileSize, 2 * tileSize, 20.5 * tileSize, 20 * tileSize);
	setScrolling(false);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	entities = new vector<infoEntities>();
	int k = 0;
	for(int j=0; j<mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			ENTITIES_TYPES type = GREEN_BLOCK;
			fin >> tile;
			map[j * mapSize.x + i] = tile;
			if (tile == 1) map[j * mapSize.x + i] = tile;
			else {
				map[j * mapSize.x + i] = 0;
				// save sprites position
				switch (tile) {
				case 2: // woods sprites
					type = WOOD;
					break;
				case 3: // orange block
					type = ORANGE_BLOCK;
					break;
				case 4: // green block
					type = GREEN_BLOCK;
					break;
				case 5: // blue block
					type = BLUE_BLOCK;
					break;
				case 6: // single coin
					type = SINGLE_COIN;
					break;
				case 7: // bags
					type = COINS_BAG;
					break;
				case 8: // multiple coins
					type = MULTIPLE_COINS;
					break;
				case 9: // diamond
					type = DIAMOND;
					break;
				case 10: // alarm
					type = ALARM;
					break;
				case 11: // axe
					type = AXE;
					break;
				case 12: //star
					type = STAR;
					break;
				}
				if (tile != 0) {
					entities->push_back(infoEntities());
					(*entities)[k].x = i;
					(*entities)[k].y = j;
					(*entities)[k].type = type;
					++k;
				}
			}
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				//texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
				texCoordTile[0] = glm::vec2(float((tile - 1) % 2) / tilesheetSize.x, float((tile - 1)/2) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void TileMap::setPlayableArea(int minxcoord, int minycoord, int maxxcoord, int maxycoord)
{
	playerArea = playableArea();
	playerArea.minx = minxcoord;
	playerArea.miny = minycoord;
	playerArea.maxx = maxxcoord;
	playerArea.maxy = maxycoord;

}

vector<bool> TileMap::reviseCollisions(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y0, y1;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	//if (y0 = y1) ++y1;
	vector<bool> collision(4, false);
	int i = 0;
	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			if (map[y * mapSize.x + x] != 0)
				collision[i] = true;
			i++;
		}
	}
	if (x0 == x1) {
		if (collision[0] || collision[1] || collision[2] || collision[3]) {
			for (int a = i + 1; a < 4; ++a) {
				collision[a] = collision[a - 2];
			}
		}
	}
	else if (y0 == y1) {
		if (collision[0] || collision[1] || collision[2] || collision[3]) {
			collision[2] = collision[1];
			collision[3] = collision[1];
			collision[1] = collision[0];
		}
	}

	return collision;
}

glm::vec2 TileMap::getNormalVector(const glm::ivec2& pos, const glm::ivec2& size, float angle) const
{
	vector<bool> c = reviseCollisions(pos, size);
	if (!c[3] && !c[2] && !c[1] && c[0]) { // Case 0001
		//return glm::vec2(1,-1);
		if(angle < 90) return glm::vec2(0, -1);
		else if (angle <= 180) return glm::vec2(1, -1);
		else return glm::vec2(1, 0);
	} 
	else if (!c[3] && !c[2] && c[1] && !c[0]) { // Case 0010
		//return glm::vec2(1, 1);
		if (angle < 180) return glm::vec2(1, 0);
		else if (angle <= 270) return glm::vec2(1, 1);
		else return glm::vec2(0, 1);
	} 
	else if(!c[3] && !c[2] && c[1] && c[0]) { // Case 0011
		return glm::vec2(1, 0);
	}
	else if (!c[3] && c[2] && !c[1] && !c[0]) { // case 0100
		//return glm::vec2(-1, -1);
		if (angle < 90) return glm::vec2(-1, -1);
		else if (angle <= 180) return glm::vec2(0, -1);
		else return glm::vec2(-1, 0);
	}
	else if (!c[3] && c[2] && !c[1] && c[0]) { // Case 0101
		return glm::vec2(0, -1);
	}
	else if (!c[3] && c[2] && c[1] && !c[0]) { // Case 0110
		if (angle < 180) return glm::vec2(-1,1);
		else return glm::vec2(1, -1);
	}
	else if (!c[3] && c[2] && c[1] && c[0]) { // Case 0111
		return glm::vec2(1, -1);
	}
	else if (c[3] && !c[2] && !c[1] && !c[0]) { // Case 1000
		//return glm::vec2(-1, 1);
		if (angle < 90) return glm::vec2(-1, 0);
		else if (angle <= 270) return glm::vec2(0, 1);
		else return glm::vec2(-1, 1);
	}
	else if (c[3] && !c[2] && !c[1] && c[0]) { // Case 1001
		if (angle < 180) return glm::vec2(1, 1);
		else return glm::vec2(-1, -1);
	}
	else if (c[3] && !c[2] && c[1] && !c[0]) { // Case 1010
		return glm::vec2(0, 1);
	}
	else if (c[3] && !c[2] && c[1] && c[0]) { // Case 1011
		return glm::vec2(1, 1);
	}
	else if (c[3] && c[2] && !c[1] && !c[0]) { // Case 1100
		return glm::vec2(-1, 0);
	}
	else if (c[3] && c[2] && !c[1] && c[0]) { // Case 1101
		return glm::vec2(-1, -1);
	}
	else if (c[3] && c[2] && c[1] && !c[0]) { // Case 1110
		return glm::vec2(-1, 1);
	}


}

bool TileMap::ballCollision(const glm::ivec2& pos, const glm::ivec2& size) const
{
	vector<bool> col = reviseCollisions(pos, size);
	bool ret = false;
	for (int i = 0; i < col.size(); i++) {
		ret |= col[i];
	}
	return ret;
}

void TileMap::setBallPos(glm::vec2 pos)
{
	ballPos = pos;
}

glm::vec2 TileMap::getBallPos()
{
	return ballPos;
}

void TileMap::setPlayerPos(glm::vec2 pos)
{
	playerPos = pos;
}

glm::vec2 TileMap::getPlayerPos()
{
	return playerPos;
}

void TileMap::setBallAngle(float angle)
{
	this->ballAngle = angle;
}

float TileMap::getBallAngle()
{
	return ballAngle;
}

void TileMap::setActualRoom(int room)
{
	actualRoom = room;
}

void TileMap::setActualLevel(int lvl)
{
	actualLevel = lvl;
}

void TileMap::setScrolling(bool b)
{
	scrolling = b;
}