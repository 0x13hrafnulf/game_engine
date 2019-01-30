#include "level.h"
#include <fstream>
#include <game_engine/errors.h>
#include <iostream>
#include <game_engine/ResourceManager.h>


level::level(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);

	//error check
	if (file.fail())
	{
		game_engine::fatalError("failed to open " + filename);
	}

	std::string tmp;
	file >> tmp >> _numHumans; //skipping first string

	std::getline(file, tmp);

	while (std::getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	game_engine::ColorRGBA8 whiteColor(255,255,255,255);

	//render all the tiles
	for (int y = 0; y < _levelData.size(); ++y)
	{
		for (int x = 0; x < _levelData[y].size(); ++x)
		{
			char tile = _levelData[y][x]; // grab the tile

			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) //process the tile
			{
				case 'R':
					_spriteBatch.draw(destRect, 
										uvRect, 
										game_engine::ResourceManager::getTexture("textures/red_bricks.png").id,
										0.0f,
										whiteColor);
					break;
				case 'G':
					_spriteBatch.draw(destRect,
						uvRect,
						game_engine::ResourceManager::getTexture("textures/glass.png").id,
						0.0f,
						whiteColor);
					break;
				case 'L':
					_spriteBatch.draw(destRect,
						uvRect,
						game_engine::ResourceManager::getTexture("textures/light_bricks.png").id,
						0.0f,
						whiteColor);
					break;
				case '@':
					_levelData[y][x] = '.';
					_startPlayerPosition.x = x * TILE_WIDTH;
					_startPlayerPosition.y = y * TILE_WIDTH;
					break;
				case 'Z':
					_levelData[y][x] = '.';
					_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					break;
				case '.':
					break;
				default:
					std::cout << "Unexpected symbol at " << x << ", " << y << std::endl;
					break;
			}
		}
	}

	_spriteBatch.end();
}


level::~level()
{
}


void level::draw()
{
	_spriteBatch.renderBatch();
}
