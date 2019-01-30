#include "player.h"
#include <SDL2/SDL.h>
#include "bullets.h"
#include "gun.h"
#include <game_engine/ResourceManager.h>
player::player()
	: _currentGunIndex(-1)
{
}


player::~player()
{
}

void player::init(float speed, glm::vec2 position, game_engine::InputManager* inputManager, game_engine::camera2D* camera, std::vector<bullets>* _Bullets)
{
	_speed = speed;
	_position = position;
	_color = game_engine::ColorRGBA8(255, 255, 255, 255);
	_inputManager = inputManager;
	_camera = camera;
	_bullets = _Bullets;
	m_textureID = game_engine::ResourceManager::getTexture("textures/player.png").id;
}


void  player::addGun(gun* _gun)
{
	_guns.push_back(_gun);
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}
}





void player::update(const std::vector<std::string>& levelData,
	std::vector<human*>& humans,
	std::vector<zombie*>& zombies,
	float deltaTime)
{
	if (_inputManager->isKeyDown(SDLK_w))
	{
		_position.y += _speed * deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_s))
	{
		_position.y -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_a))
	{
		_position.x -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_d))
	{
		_position.x += _speed * deltaTime;
	}

	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2)
	{
		_currentGunIndex = 2;
	}
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	m_direction = glm::normalize(mouseCoords - centerPosition);


	if (_currentGunIndex != -1)
	{
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
										centerPosition,
										m_direction,
										*_bullets, deltaTime);
	}

	collideWithLevel(levelData);
}