#pragma once

#include "human.h"
#include <game_engine/InputManager.h>
#include <game_engine/camera2D.h>

class gun;
class bullets;

class player : public human
{
public:
	player();
	~player();

	void init(float speed, glm::vec2 position, game_engine::InputManager* inputManager, game_engine::camera2D* camera, std::vector<bullets>* _Bullets);
	void update(const std::vector<std::string>& levelData,
		std::vector<human*>& humans,
		std::vector<zombie*>& zombies, 
		float deltaTime);
	void addGun(gun* _gun);
private:
	std::vector<gun*> _guns;
	int _currentGunIndex;
	
	std::vector<bullets>* _bullets;

	game_engine::camera2D* _camera;
	game_engine::InputManager* _inputManager;
};

