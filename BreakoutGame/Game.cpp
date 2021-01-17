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
}

void Game::Update(float dt) 
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{
	renderer->DrawSprite((Texture2D&)ResourceManager::GetTexture("face"),
		glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}
