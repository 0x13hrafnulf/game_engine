#include "bullets.h"
#include <game_engine/ResourceManager.h>
#include "agent.h"
#include "zombie.h"
#include "human.h"
#include "level.h"

bullets::bullets(glm::vec2 position, glm::vec2 direction, float damage, float speed)
	:
	_damage(damage),
	_position(position),
	_direction(direction),
	_speed(speed)
{
}


bullets::~bullets()
{
}


bool bullets::update(const std::vector<std::string>& levelData, float deltaTime)
{
	_position += _direction * _speed * deltaTime;
	return collideWithWorld(levelData);
}


void bullets::draw(game_engine::spriteBatch& _spriteBatch)
{
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	game_engine::ColorRGBA8 _color(75, 75, 75, 255);
	_spriteBatch.draw(destRect, uvRect, game_engine::ResourceManager::getTexture("textures/circle.png").id, 0.0f, _color);

}


bool bullets::collideWithAgent(agent* _agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = _agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		return true;
	}
	return false;

}


bool bullets::collideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size()
		|| gridPosition.y < 0 || gridPosition.y >= levelData.size())
	{
		return true;
	}

	return levelData[gridPosition.y][gridPosition.x] != '.';
}