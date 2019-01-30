#pragma once

#include <vector>
#include <string>

#include <game_engine/spriteBatch.h>
#include <game_engine/ResourceManager.h>


const int TILE_WIDTH = 64;

class level
{
public:
	//load the level

	level(const std::string& filename);
	~level();

	void draw();
	const std::vector<std::string>& getLevelData() const { return _levelData; }
	glm::vec2 getStartPlayerPos() const { return _startPlayerPosition; }
	const std::vector<glm::vec2> getStartZombiePos() const { return _zombieStartPositions; }
	int getNumHumans() const { return _numHumans; }
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }

private:

	int _numHumans;

	std::vector<std::string> _levelData;
	game_engine::spriteBatch _spriteBatch;


	glm::vec2 _startPlayerPosition;
	std::vector<glm::vec2> _zombieStartPositions;
};

