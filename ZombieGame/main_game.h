#pragma once

#include <game_engine/GLSLProgram.h>
#include <game_engine/window.h>
#include <game_engine/camera2D.h>
#include <game_engine/InputManager.h>
#include <game_engine/spriteBatch.h>
#include <game_engine/SpriteFont.h>
#include <game_engine/audio_engine.h>
#include <game_engine/particle_engine2D.h>
#include <game_engine/particle_batch2D.h>

#include "level.h"
#include "agent.h"
#include "player.h"
#include "bullets.h"

class zombie;

enum class gameState
{
	PLAY, EXIT
};
class main_game
{
public:
	main_game();
	~main_game();

	void run();

private:
	

	void initSystems();

	void initLevel();

	void initShaders();

	void gameLoop();
	void checkVictory();
	void processInput();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void drawGame();

	void addBlood(const glm::vec2& postion, int numParticles);

	void drawHud();

	game_engine::window _window;
	game_engine::GLSLProgram _textureProgram;
	game_engine::InputManager _inputManager;
	game_engine::camera2D _camera;
	game_engine::camera2D _hudCamera;
	std::vector<level*> _levels; // vector of all levels
	std::vector<human*> _humans; // vector of all humans
	std::vector<zombie*> _zombies; // vector of all humans
	std::vector<bullets> _bullets;
	player* _player;

	float _fps;
	int _currentLevel;
	int _screenWidth, _screenHeight;
	gameState _gameState;


	int _numHumansKilled;
	int _numZombiesKilled;

	game_engine::spriteBatch _agentSpriteBatch;
	game_engine::spriteBatch _hudSpriteBatch;
	game_engine::audio_engine m_audioEngine;
	game_engine::SpriteFont* _spriteFont;
	game_engine::particle_engine2D m_particleEngine;//need to be pointer?
	game_engine::particle_batch2D* m_bloodParticleBatch;

};

