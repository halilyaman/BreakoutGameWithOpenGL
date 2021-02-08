#pragma once

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include <vector>
#include <tuple>

namespace enums
{
	enum GameState
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};

	enum Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
}



typedef std::tuple<bool, enums::Direction, glm::vec2> Collision;

class Game
{
public:
	enums::GameState state_;
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
	Collision CheckCollision(BallObject& obj_1, GameObject& obj_2);
	void DoCollisionCheck();
	enums::Direction VectorDirection(glm::vec2 target);
	void ResetLevel();
	void ResetPlayer();
};

