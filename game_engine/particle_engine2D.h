#pragma once

#include <vector>


namespace game_engine {

	class particle_batch2D;
	class spriteBatch;


	class particle_engine2D
	{
	public:



		particle_engine2D();
		~particle_engine2D();


		//after adding particle batch, particleengine becomes responsible for deleting
		void addPartcleBatch(particle_batch2D* particleBatch);
		void update(float deltaTime);
		void draw(spriteBatch* SpriteBatch);


	private:

		std::vector<particle_batch2D*> m_batches;



	};


}