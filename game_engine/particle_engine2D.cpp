#include "particle_engine2D.h"
#include "spriteBatch.h"
#include "particle_batch2D.h"

namespace game_engine {


	particle_engine2D::particle_engine2D()
	{
		//Empty
	}


	particle_engine2D::~particle_engine2D()
	{
		for (auto& b : m_batches)
		{
			delete b;
		}


	}

	void particle_engine2D::addPartcleBatch(particle_batch2D* particleBatch)
	{
		m_batches.push_back(particleBatch);
	}
	void particle_engine2D::update(float deltaTime)
	{
		for (auto& b : m_batches)
		{
			b->update(deltaTime);
		}
	}
	void particle_engine2D::draw(spriteBatch* SpriteBatch)
	{
		for (auto& b : m_batches)
		{
			SpriteBatch->begin();
			b->draw(SpriteBatch);
			SpriteBatch->end();
			SpriteBatch->renderBatch();
		}
	}
}