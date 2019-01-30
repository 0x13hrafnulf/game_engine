#pragma once

#include "agent.h"

class zombie : public agent
{
public:
	zombie();
	~zombie();
	void init(float speed, glm::vec2 position);
	virtual void update(const std::vector<std::string>& levelData,
		std::vector<human*>& humans,
		std::vector<zombie*>& zombies,
		float deltaTime) override;



private:
	human* getNearestHuman(std::vector<human*>& humans);
};

