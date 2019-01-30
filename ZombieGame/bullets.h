#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <game_engine/spriteBatch.h>
class human;
class zombie;
class agent;

const int BULLET_RADIUS = 5;


class bullets
{
public:
	bullets(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~bullets();

	bool update(const std::vector<std::string>& levelData, float deltaTime);

	bool collideWithAgent(agent* _agent);

	float getDamage() const { return _damage; }

	void draw(game_engine::spriteBatch& _spriteBatch);
	glm::vec2 getPosition() const { return _position;  }

private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;

};

