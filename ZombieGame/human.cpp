#include "human.h"
#include <random>
#include <ctime>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <game_engine/ResourceManager.h>

human::human()
	: _frames(0)
{
}


human::~human()
{
}
void human::init(float speed, glm::vec2 position)
{
	static std::mt19937 randomEngine(time(nullptr));

	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_health = 20;

	_color = game_engine::ColorRGBA8(255, 255, 255, 255);




	_speed = speed;
	_position = position;

	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);

	m_textureID = game_engine::ResourceManager::getTexture("textures/human.png").id;
}




void human::update(const std::vector<std::string>& levelData,
	std::vector<human*>& humans,
	std::vector<zombie*>& zombies,
	float deltaTime)
{
	
	const float PI_CONST = 3.14159265359f;

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f * PI_CONST / 180, 40.0f * PI_CONST / 180);

	_position += m_direction * _speed * deltaTime;

	if (_frames == 20)
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else ++_frames;

	if (collideWithLevel(levelData))
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}
}
