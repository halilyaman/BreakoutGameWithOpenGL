#pragma once

#include <vector>
#include "GameObject.h"
#include "SpriteRenderer.h"

class GameLevel
{
public:
	// level state
	std::vector<GameObject> bricks_;
	// constructor
	GameLevel() { }
	// loads level from file
	void Load(const char* file, unsigned int level_width, unsigned int level_heigth);
	// render level
	void Draw(SpriteRenderer& renderer);
	// check if level is completed (all non-solid tiles are destroyed)
	bool IsCompleted();
private:
	// initialize level from tile data
	void Init(
		std::vector<std::vector<unsigned int>> tile_data,
		unsigned int level_width,
		unsigned int level_height
	);
};

