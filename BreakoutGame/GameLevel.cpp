#include "GameLevel.h"
#include <fstream>
#include <sstream>
#include "ResourceManager.h"

void GameLevel::Load(const char* file, unsigned int level_width, unsigned int level_height)
{
	// clear old data
	this->bricks_.clear();
	// load from file
	unsigned int tile_code;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tile_data;
	if (fstream)
	{
		while (std::getline(fstream, line)) // read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tile_code) // read each word seperated by spaces
			{
				row.push_back(tile_code);
			}
			tile_data.push_back(row);
		}
		if (tile_data.size() > 0)
		{
			this->Init(tile_data, level_width, level_height);
		}
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->bricks_)
	{
		if (!tile.is_destroyed_)
		{
			tile.Draw(renderer);
		}
	}
}

bool GameLevel::IsCompleted()
{
	for (GameObject& tile : this->bricks_)
	{
		if (!tile.is_solid_ && !tile.is_destroyed_)
		{
			return false;
		}
	}
	return true;
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height)
{
	// calculate dimensions
	unsigned int height = tile_data.size();
	unsigned int width = tile_data[0].size();
	float unit_width = static_cast<float>(level_width) / width;
	float unit_height = static_cast<float>(level_height) / height;
	// initialize level tiles based on tile_data
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			// check block type from level data (2D level array)
			if (tile_data[y][x] == 1) // solid
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(
					pos,
					size,
					ResourceManager::GetTexture("block_solid"),
					glm::vec3(0.8f, 0.8f, 0.7f)
				);
				obj.is_solid_ = true;
				this->bricks_.push_back(obj);
			}
			else if (tile_data[y][x] > 1) // non-solid; determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tile_data[y][x] == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tile_data[y][x] == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tile_data[y][x] == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tile_data[y][x] == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}
				glm::vec2 pos(x * unit_width, y * unit_height);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(
					pos,
					size,
					ResourceManager::GetTexture("block"),
					color
				);
				this->bricks_.push_back(obj);
			}
		}
	}
}