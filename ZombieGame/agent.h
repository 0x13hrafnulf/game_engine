#pragma once

#include <glm/glm.hpp>
#include <game_engine/spriteBatch.h>

class zombie;
class human;

const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;



class agent
{
public:
	agent();
	virtual ~agent();

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<human*>& humans, 
						std::vector<zombie*>& zombies,
						float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	void draw(game_engine::spriteBatch& _spriteBatch);

	glm::vec2 getPosition() const { return _position; }

	bool collideWithAgent(agent* _agent);

	bool applyDamage(float damage);
protected:
	
	void checkTilePosition(const std::vector<std::string>& levelData, 
							std::vector<glm::vec2>& collideTilePosition,
											float x, float y);

	void collideWithTile(glm::vec2 tilePos);
	glm::vec2 _position;
	float _speed;
	game_engine::ColorRGBA8 _color;
	float _health;
	GLuint m_textureID;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
};

