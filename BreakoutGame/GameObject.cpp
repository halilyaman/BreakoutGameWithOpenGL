#include "GameObject.h"

GameObject::GameObject() :
	position_(0.0f, 0.0f),
	velocity_(0.0f, 0.0f),
	size_(1.0f, 1.0f),
	color_(1.0f, 1.0f, 1.0f),
	rotation_(0.0f),
	sprite_(),
	is_solid_(false),
	is_destroyed_(false) { }

GameObject::GameObject(
	glm::vec2 position,
	glm::vec2 size,
	Texture2D sprite,
	glm::vec3 color,
	glm::vec2 velocity) 
	: position_(position), size_(size), sprite_(sprite), color_(color), rotation_(0.0f), velocity_(velocity),
		is_solid_(false), is_destroyed_(false) { }

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(
		this->sprite_,
		this->position_,
		this->size_,
		this->rotation_,
		this->color_
	);
}