#include "particle_batch2D.h"

namespace game_engine
{
	

	particle_batch2D::particle_batch2D()
	{
		//Empty
	}


	particle_batch2D::~particle_batch2D()
	{
		delete[] m_particles;
	}

	void particle_batch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(particle2D&, float)> updateFunc)
	{
		m_decayRate = decayRate;
		m_maxParticles = maxParticles;
		m_particles = new particle2D[m_maxParticles];
		m_texture = texture;
		m_updateFunc = updateFunc;
	}
	void particle_batch2D::addParticle(const glm::vec2& position,
										const glm::vec2& velocity,
										const ColorRGBA8 color,
										float width)
	{
		int particleIndex = findFreeParticle();

		auto& p = m_particles[particleIndex];

		p.life = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;

	}
	void  particle_batch2D::update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; ++i)
		{
			if (m_particles[i].life > 0.0f)
			{
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].life -= m_decayRate * deltaTime;
			}
		}
	}
	void particle_batch2D::draw(spriteBatch* SpriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; ++i)
		{
			auto& p = m_particles[i];
			if (m_particles[i].life > 0.0f)
			{
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				SpriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
			}
		}
	}

	int particle_batch2D::findFreeParticle()
	{
		for (int i = m_lastFreeParticle; i < m_maxParticles; ++i)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastFreeParticle; ++i)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}
		//no free particles, override 1st one
		return 0;
	}
}