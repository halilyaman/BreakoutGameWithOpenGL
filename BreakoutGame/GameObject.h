#pragma once

#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	//  object state
	glm::vec2 position_, size_, velocity_;
	glm::vec3 color_;
	float rotation_;
	bool is_solid_;
	bool is_destroyed_;
	// render state
	Texture2D sprite_;
	// constructors
	GameObject();
	GameObject(
		glm::vec2 position,
		glm::vec2 size,
		Texture2D sprite,
		glm::vec3 color = glm::vec3(1.0f),
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f)
	);
	// draw sprite
	virtual void Draw(SpriteRenderer& renderer);
};

