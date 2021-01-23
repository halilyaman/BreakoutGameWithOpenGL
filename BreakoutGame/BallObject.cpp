#include "BallObject.h"

BallObject::BallObject() : GameObject(), radius_(12.5f), stuck_on_ball_(true) {}

BallObject::BallObject(
	glm::vec2 pos,
	float radius,
	glm::vec2 velocity, 
	Texture2D sprite) : GameObject(
		pos,
		glm::vec2(radius * 2.0f, radius * 2.0f),
		sprite,
		glm::vec3(1.0f),
		velocity), radius_(radius), stuck_on_ball_(true) {}

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
	if (!this->stuck_on_ball_)
	{
		// move the ball
		this->position_ += this->velocity_ * dt;
		// check if outside window bounds; 
		// if so, reverse velocity and restore at correct position
		if (this->position_.x <= 0.0f)
		{
			this->velocity_.x = -(this->velocity_.x);
			this->position_.x = 0.0f;
		}
		else if (this->position_.x + this->size_.x >= window_width)
		{
			this->velocity_.x = -(this->velocity_.x);
			this->position_.x = window_width - this->size_.x;
		}
		if (this->position_.y <= 0.0f)
		{
			this->velocity_.y = -(this->velocity_.y);
			this->position_.y = 0.0f;
		}
	}
	return this->position_;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->position_ = position;
	this->velocity_ = velocity;
	this->stuck_on_ball_ = true;
}