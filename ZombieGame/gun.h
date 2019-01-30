#pragma once


#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "bullets.h"
#include <game_engine/audio_engine.h>

class gun
{
public:
	gun(std::string name, int firerate, int bulletsPerShot, 
		float spread, float bulletDmg, float bulletSpd,
		game_engine::sound_effect fireEffect);
	~gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<bullets>& _bullets, float deltaTime);

private:
	void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<bullets>& _bullets);
	std::string _name;

	int _fireRate; //fire rate in terms of frames

	float _spread; // accuracy

	game_engine::sound_effect m_fireEffect;

	int _bulletsPerShot;
	float _bulletDamage;
	float _bulletSpeed;

	float _frameCount; 
};

