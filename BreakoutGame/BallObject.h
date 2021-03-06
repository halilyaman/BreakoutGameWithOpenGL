#pragma once

#include <glm/glm.hpp>
#include "GameObject.h"

class BallObject : public GameObject
{
public:
	// ball state
	float radius_;
	bool stuck_on_paddle_;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 Move(float dt, unsigned int window_width);
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

