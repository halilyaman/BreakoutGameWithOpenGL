#include "Game.h"
#include <GLFW/glfw3.h>
#include "BallObject.h"

// game-related state data
SpriteRenderer* renderer;
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
GameObject* player;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS(12.5f);
BallObject* ball;

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
