#include "agent.h"
#include "level.h"
#include <game_engine/ResourceManager.h>
#include <algorithm>

agent::agent()
{
}


agent::~agent()
{
}

bool agent::collideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePosition;
	//check the 4 corners

	checkTilePosition(levelData, collideTilePosition, _position.x, _position.y);
	checkTilePosition(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y);
	checkTilePosition(levelData, collideTilePosition, _position.x, _position.y + AGENT_WIDTH);
	checkTilePosition(levelData, collideTilePosition, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);
	
	if (collideTilePosition.size() == 0)
	{
		return false;
	}


	for (int i = 0; i < collideTilePosition.size(); ++i)
	{
		collideWithTile(collideTilePosition[i]);
	}
	return true;
}


void agent::checkTilePosition(const std::vector<std::string>& levelData, 
	std::vector<glm::vec2>& collideTilePosition, float x, float y)
{
	glm::vec2 _cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	if (levelData[_cornerPos.y][_cornerPos.x] != '.')
	{
		collideTilePosition.push_back(_cornerPos  * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH/2.0f));
	}

	if (_cornerPos.x < 0 || _cornerPos.x >= levelData[0].size() 
		|| _cornerPos.y < 0 || _cornerPos.y >= levelData.size())
	{
		return;
	}


}
//AABB collision
void agent::collideWithTile(glm::vec2 tilePos)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	
	
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;


	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);


	if (xDepth > 0 && yDepth > 0)
	{
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				_position.x -= xDepth;
			}
			else _position.x += xDepth;
		}
		else
		{
			if (distVec.y < 0)
			{
				_position.y -= yDepth;
			}
			else _position.y += yDepth;
			
		}
	}

}


//circular collision
bool agent::collideWithAgent(agent* _agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;


	
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = _agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepVec = glm::normalize(distVec) * collisionDepth;
		
		_position += collisionDepVec / 2.0f;
		_agent->_position -= collisionDepVec / 2.0f;
		return true;
	}
	return false;
}


void agent::draw(game_engine::spriteBatch& _spriteBatch)
{

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;


	_spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, _color, m_direction);
}

bool agent::applyDamage(float damage)
{
	_health -= damage;

	if (_health <= 0)
	{
		return true;
	}
	return false;
}