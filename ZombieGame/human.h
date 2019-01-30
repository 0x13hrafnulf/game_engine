#pragma once

#include "agent.h"

class human : public agent
{
public:
	human();
	virtual ~human();

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<human*>& humans,
		std::vector<zombie*>& zombies,
		float deltaTime) override;

	void init(float speed, glm::vec2 position);


private:
	int _frames;
};

