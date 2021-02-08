#include "Game.h"
#include <GLFW/glfw3.h>

// game-related state data
static SpriteRenderer* renderer;
static const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
static const float PLAYER_VELOCITY(500.0f);
static GameObject* player;
static const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
static const float BALL_RADIUS(12.5f);
static BallObject* ball;

Game::Game(unsigned int width, unsigned int height)
	: width_(width), height_(height), state_(GAME_ACTIVE), keys_(), level_(0) { }

Game::~Game()
{
	delete renderer;
}

void Game::Init() 
{
	// load shaders
	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width_), static_cast<float>(this->height_), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	// set render-specific controls
	renderer = new SpriteRenderer((Shader&)ResourceManager::GetShader("sprite"));
	// load textures
	ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("textures/block.png", false, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
	// load levels
	GameLevel level_1;
	GameLevel level_2;
	GameLevel level_3;
	GameLevel level_4;
	level_1.Load("levels/1.lvl", this->width_, this->height_ / 2);
	level_2.Load("levels/2.lvl", this->width_, this->height_ / 2);
	level_3.Load("levels/3.lvl", this->width_, this->height_ / 2);
	level_4.Load("levels/4.lvl", this->width_, this->height_ / 2);
	this->levels_.push_back(level_1);
	this->levels_.push_back(level_2);
	this->levels_.push_back(level_3);
	this->levels_.push_back(level_4);
	this->level_ = 0;
	// load player
	glm::vec2 player_pos(
		this->width_ / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->height_ - PLAYER_SIZE.y
	);
	player = new GameObject(player_pos, PLAYER_SIZE, (Texture2D&) ResourceManager::GetTexture("paddle"));
	// load ball
	glm::vec2 ball_pos = player_pos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	ball = new BallObject(
		ball_pos,
		BALL_RADIUS,
		INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));
}

void Game::Update(float dt) 
{
	ball->Move(dt, this->width_);
	this->DoCollisionCheck();
}

void Game::ProcessInput(float dt)
{
	float velocity = dt * PLAYER_VELOCITY;
	if (this->state_ == GAME_ACTIVE)
	{
		if (this->keys_[GLFW_KEY_A])
		{
			if (player->position_.x >= 0.0f)
			{
				player->position_.x -= velocity;
				if (ball->stuck_on_ball_)
				{
					ball->position_.x -= velocity;
				}
			}
		}
		if (this->keys_[GLFW_KEY_D])
		{
			if (player->position_.x <= this->width_ - player->size_.x)
			{
				player->position_.x += velocity;
				if (ball->stuck_on_ball_)
				{
					ball->position_.x += velocity;
				}
			}
		}
		if (this->keys_[GLFW_KEY_SPACE])
		{
			ball->stuck_on_ball_ = false;
		}
	}
}

void Game::Render()
{
	if (this->state_ == GAME_ACTIVE)
	{
		// draw background
		renderer->DrawSprite(
			(Texture2D&) ResourceManager::GetTexture("background"),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(this->width_, this->height_),
			0.0f
		);
		// draw level
		this->levels_[this->level_].Draw(*renderer);
		// draw player
		player->Draw(*renderer);
		// draw ball
		ball->Draw(*renderer);
	}
}

Collision Game::CheckCollision(BallObject& obj_1, GameObject& obj_2)
{
	// get center of the ball
	glm::vec2 center(obj_1.position_ + obj_1.radius_);
	// calculate aabb center and half-extents
	glm::vec2 aabb_half_extents(obj_2.size_.x / 2.0f, obj_2.size_.y / 2.0f);
	glm::vec2 aabb_center(
		obj_2.position_.x + aabb_half_extents.x,
		obj_2.position_.y + aabb_half_extents.y
	);
	// get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// add clamped value to aabb_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// retrieve vector between center circle and closest point aabb and check if length <= radius
	difference = closest - center;

	if (glm::length(difference) <= obj_1.radius_)
	{
		return std::make_tuple(true, VectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
	}
}

void Game::DoCollisionCheck()
{
	for (GameObject& box : this->levels_[this->level_].bricks_)
	{
		if (!box.is_destroyed_)
		{
			Collision collision = CheckCollision(*ball, box);

			if (std::get<0>(collision))
			{
				// destroy block if not solid
				if (!box.is_solid_)
				{
					box.is_destroyed_ = true;
				}
				// collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // horizontal collision
				{
					ball->velocity_.x = -ball->velocity_.x; // reverse horizontal velocity
					// relocate
					float penetration = ball->radius_ - std::abs(diff_vector.x);
					if (dir == LEFT)
					{
						ball->position_.x += penetration; // move ball to rigth
					}
					else
					{
						ball->position_.x -= penetration; // move ball to left
					}
				}
				else // vertical collision
				{
					ball->velocity_.y = -ball->velocity_.y; // reverse vertical velocity
					float penetration = ball->radius_ - std::abs(diff_vector.y);
					if (dir == UP)
					{
						ball->position_.y -= penetration; // move ball back up
					}
					else
					{
						ball->position_.y += penetration; // move ball back down
					}
				}
			}
		}
	}

	// collision between ball and palette
	Collision collision = CheckCollision(*ball, *player);
	if (std::get<0>(collision))
	{
		ball->velocity_.y = -ball->velocity_.y;
	}
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] =
	{
		glm::vec2( 0.0f,  1.0f),
		glm::vec2( 1.0f,  0.0f),
		glm::vec2( 0.0f, -1.0f),
		glm::vec2(-1.0f,  0.0f)
	};

	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}