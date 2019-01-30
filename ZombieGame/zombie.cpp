#include "zombie.h"
#include "human.h"
#include <game_engine/ResourceManager.h>

zombie::zombie()
{
}


zombie::~zombie()
{
}

void zombie::init(float speed, glm::vec2 position)
{
	_color = game_engine::ColorRGBA8(0, 0, 255, 255);
	_health = 150;
	_speed = speed;
	_position = position;
	m_textureID = game_engine::ResourceManager::getTexture("textures/zombie.png").id;

}
human* zombie::getNearestHuman(std::vector<human*>& humans)
{
	human* closestHuman = nullptr;
	float smallestDist = 999999999.0f;

	for (int i = 0; i < humans.size(); ++i)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDist)
		{
			smallestDist = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}
void zombie::update(const std::vector<std::string>& levelData,
	std::vector<human*>& humans,
	std::vector<zombie*>& zombies,
	float deltaTime)
{
	human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr)
	{
		m_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += m_direction * _speed * deltaTime;
	}


	collideWithLevel(levelData);
	
}	