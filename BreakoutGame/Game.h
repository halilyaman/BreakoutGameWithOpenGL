#pragma once

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include <vector>

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	GameState state_;
	bool keys_[1024];
	unsigned int width_, height_;
	std::vector<GameLevel> levels_;
	unsigned int level_;

	// constructor / destructor
	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
};

