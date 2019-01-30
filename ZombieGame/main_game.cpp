#include "main_game.h"
#include <iostream>
#include <game_engine/game_engine.h>
#include <game_engine/Timing.h>
#include "zombie.h"
#include <random>
#include <ctime>
#include <game_engine/errors.h>
#include "gun.h"
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;

main_game::main_game()
	:
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(gameState::PLAY),
	_fps(0),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0)
{
}

main_game::~main_game()
{

	for (int i = 0; i < _levels.size(); ++i)
	{
		delete _levels[i];
	}
	for (int i = 0; i < _humans.size(); ++i)
	{
		delete _humans[i];
	}
}

void main_game::run()
{
	initSystems();
	initLevel();

	game_engine::music mus = m_audioEngine.loadMusic("sound/XYZ.ogg");
	mus.play(-1);

	gameLoop();

}
void updateBloodParticle(game_engine::particle2D& particle, float deltaTime)
{
	particle.position += particle.velocity * deltaTime;
	particle.color.a = GLubyte(particle.life * 255.0f);
}
void main_game::initSystems()
{
	game_engine::init();
	//sound must be init after game engine init
	m_audioEngine.init();

	_window.create("Zombie_game", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initShaders();

	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);
	_hudCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

	_agentSpriteBatch.init();
	_hudSpriteBatch.init();
	
	//after shaders and opengl =>init the sprite font

	_spriteFont = new game_engine::SpriteFont("fonts/chintzy.ttf", 64);

	_levels.push_back(new level("levels/level1.txt"));
	_currentLevel = 0;

	//init particles

	m_bloodParticleBatch = new game_engine::particle_batch2D;
	m_bloodParticleBatch->init(1000, 0.05f, 
		game_engine::ResourceManager::getTexture("textures/particle.png"),
		updateBloodParticle);/*[] (game_engine::particle2D& particle, float deltaTime) {
							 particle.position += particle.velocity * deltaTime;
							 particle.color.a = GLubyte(particle.life * 255.0f);} */
	m_particleEngine.addPartcleBatch(m_bloodParticleBatch);

}

void main_game::initLevel()
{
	//level 1
	_levels.push_back(new level("levels/level1.txt"));
	_currentLevel = 0;

	_player = new player;
	_player->init(5.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	//add all random humans
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(1, _levels[_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(1, _levels[_currentLevel]->getHeight() - 2);


	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); ++i)
	{

		_humans.push_back(new human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);

	}
	// add the zombies
	
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getStartZombiePos();
	for (int i = 0; i < zombiePositions.size(); ++i)
	{
		_zombies.push_back(new zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);

	}


	//set up guns

	const float BULLET_SPEED = 20.0f;
	_player->addGun(new gun("Magnum", 10, 1, 5.0f, 30.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("sound/shots/pistol.wav")));
	_player->addGun(new gun("Shotgun", 30, 12, 20.0f, 15.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("sound/shots/shotgun.wav")));
	_player->addGun(new gun("MP5", 2, 1, 10.0f, 20.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("sound/shots/cg1.wav")));

}

void main_game::initShaders()
{
	_textureProgram.compileShaders("shaders/colorShadingVert.txt", "shaders/colorShadingFrag.txt");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void main_game::gameLoop()
{
	game_engine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float CAMERA_SCALE = 1.0f / 3.0f;
	const float MAX_DELTATIME = 1.0f;
	_camera.setScale(CAMERA_SCALE);

	float prevTicks = SDL_GetTicks();

	while (_gameState == gameState::PLAY)
	{
		float newTicks = SDL_GetTicks();
		float frametime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frametime / DESIRED_FRAMETIME;



		fpsLimiter.begin();

		checkVictory();

		_inputManager.update();

		processInput();

		int i = 0;//dealing with timestep using semi-fixed
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			++i;
		}

		

		_camera.setPosition(_player->getPosition());
		_camera.update();
		_hudCamera.update();


		drawGame();
		_fps = fpsLimiter.end();
		std::cout << _fps << std::endl;
	}

}

void main_game::checkVictory()
{
	//support for multiple levels ++currentLevel
	if (_zombies.empty())
	{
		//print out the stats 


		game_engine::fatalError("YOU WIN!");
	}


}

void main_game::updateAgents(float deltaTime)
{
	for (int i = 0; i < _humans.size(); ++i)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), 
			_humans,
			_zombies, deltaTime
			);
	}

	for (int i = 0; i < _zombies.size(); ++i)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime
		);
	}
	//update zombie collisions
	for (int i = 0; i < _zombies.size(); ++i)
	{
		for (int j = i + 1; j < _zombies.size(); ++j)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		for (int j = 1; j < _humans.size(); ++j)//skipping player
		{
			if (_zombies[i]->collideWithAgent(_humans[j]))
			{
				_zombies.push_back(new zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		// collide with player
		if (_zombies[i]->collideWithAgent(_player))
		{
			game_engine::fatalError("You lose!");
		}

	}




	//update human collisions
	for (int i = 0; i < _humans.size(); ++i)
	{
		for (int j = i + 1; j < _humans.size(); ++j)
		{
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}


	//dont forget to update zombies
}

void main_game::updateBullets(float deltaTime)
{
	for (int i = 0; i < _bullets.size();)
	{
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime))
		{
			//collided with wall
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
		{
			++i;
		}
	}

	bool bulletRemoved = false;


	for (int i = 0; i < _bullets.size(); ++i)
	{
		for (int j = 0; j < _zombies.size();)
		{
			if (_bullets[i].collideWithAgent(_zombies[j]))
			{
				addBlood(_bullets[i].getPosition(), 5);
				
				//damage the zombie
				if (_zombies[j]->applyDamage(_bullets[i].getDamage()))
				{
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					++_numZombiesKilled;
				}
				else
				{
					++j;
				}
				
				//remove the bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				bulletRemoved = true;// not to skip bullets
				break; //bullet died, no need to loop 
			}
			else
			{
				++j;
			}
		}
	// loop through humans

	if (bulletRemoved == true) continue;

	for (int j = 1; j < _humans.size();)
	{
		if (_bullets[i].collideWithAgent(_humans[j]))
		{
			addBlood(_bullets[i].getPosition(), 5);
			//damage the human
			if (_humans[j]->applyDamage(_bullets[i].getDamage()))
			{
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
				++_numHumansKilled;
			}
			else
			{
				++j;
			}

			//remove the bullet
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			--i;// not to skip bullets
			break; //bullet died, no need to loop 
		}
		else
		{
			++j;
		}
	}
}
}

void main_game::processInput()
{
	SDL_Event evnt;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			//Exit the game here!
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}

	}

}

void main_game::drawGame()
{
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//draw code goes here
	glActiveTexture(GL_TEXTURE0);
	//make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw the level
	_levels[_currentLevel]->draw();
	//draw the humans
	_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);


	for (int i = 0; i < _humans.size(); ++i)
	{
		if(_camera.isBoxInView(_humans[i]->getPosition(), agentDims))
			_humans[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _zombies.size(); ++i)
	{
		if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims))
			_zombies[i]->draw(_agentSpriteBatch);
	}
	for (int i = 0; i < _bullets.size(); ++i)
	{
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();
	//Render the hud display
	m_particleEngine.draw(&_agentSpriteBatch);


	drawHud();

	_textureProgram.unuse();

	_window.swapBuffer();
}


void main_game::drawHud()
{
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);



	_hudSpriteBatch.begin();

	sprintf_s(buffer, "Num humans %d", _humans.size());

	_spriteFont->draw(_hudSpriteBatch, buffer, 
		glm::vec2(0, 0), glm::vec2(0.5f), 0.0f, 
		game_engine::ColorRGBA8(255, 255, 255, 255));
	
	sprintf_s(buffer, "Num zombies %d", _zombies.size());

	_spriteFont->draw(_hudSpriteBatch, buffer,
		glm::vec2(0, 36), glm::vec2(0.5f), 0.0f,
		game_engine::ColorRGBA8(255, 255, 255, 255));


	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}


void main_game::addBlood(const glm::vec2& postion, int numParticles)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	glm::vec2 vel(2.0f, 0.0f);
	game_engine::ColorRGBA8 col(255, 0, 0, 255);
	for(int i = 0; i < numParticles; ++i)
	{
		m_bloodParticleBatch->addParticle(postion, glm::rotate(vel, randAngle(randEngine)), col, 30.0f);
	}
}