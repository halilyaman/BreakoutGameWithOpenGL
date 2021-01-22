#include "Game.h"

// game-related state data
SpriteRenderer* renderer;

Game::Game(unsigned int width, unsigned int height)
	: width_(width), height_(height), state_(GAME_ACTIVE), keys_()
{

}

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
}

void Game::Update(float dt) 
{

}

void Game::ProcessInput(float dt)
{

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
	}
}
