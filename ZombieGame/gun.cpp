#include "gun.h"

#include <random>
#include <ctime>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

gun::gun(std::string name, int firerate, int bulletsPerShot, float spread, float bulletDmg, float bulletSpd, game_engine::sound_effect fireEffect)
	:
	_name(name),
	_fireRate(firerate),
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletDamage(bulletDmg),
	_bulletSpeed(bulletSpd),
	_frameCount(0),
	m_fireEffect(fireEffect)
{


}


gun::~gun()
{
}

void gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<bullets>& _bullets, float deltaTime)
{
	_frameCount+= deltaTime;
	if (_frameCount >= _fireRate && isMouseDown)
	{
		fire(position, direction, _bullets);
		_frameCount = 0;
	}
}


void gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<bullets>& _bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	m_fireEffect.play();
	
	for (int i = 0; i < _bulletsPerShot; ++i)
	{
		_bullets.emplace_back(position, 
			glm::rotate(direction, glm::radians(randRotate(randomEngine))),
			_bulletDamage, _bulletSpeed);

	}

}

