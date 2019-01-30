#pragma once
#include <glm/glm.hpp>
#include "vertex.h"
#include "spriteBatch.h"
#include "GLTexture.h"

#include <functional>//function pointers


namespace game_engine
{
	
	class particle2D
	{
	
	public:

		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float life = 0.0f;
		float width = 0.0f;
	};

	inline void DefaultParticleUpdate(particle2D& particle, float deltaTime)
	{
		particle.position += particle.velocity * deltaTime;
	}

	class particle_batch2D
	{
	public:
		

		particle_batch2D();
		~particle_batch2D();
		void init(int maxParticles, float decayRate, GLTexture texture,
					std::function<void(particle2D&, float)> updateFunc = DefaultParticleUpdate);
		void addParticle(const glm::vec2& position, 
						 const glm::vec2& velocity,
						 const ColorRGBA8 color, float width);
	
		void update(float deltaTime);
		void draw(spriteBatch* SpriteBatch);
	private:
		int findFreeParticle();


		float m_decayRate = 0.1f;
		particle2D* m_particles = nullptr;
		int m_maxParticles = 0;
		GLTexture m_texture;
		int m_lastFreeParticle = 0;

		std::function<void(particle2D&, float)> m_updateFunc;
	};
}
