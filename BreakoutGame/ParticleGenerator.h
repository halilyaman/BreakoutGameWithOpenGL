#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"

struct Particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	float     life;
	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

class ParticleGenerator
{
public:
	// constructor
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
	// update all particles
	void Update(float dt, GameObject& object, unsigned int new_particles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// render all particles
	void Draw();
private:
	// state
	std::vector<Particle> particles;
	unsigned int amount;
	// render state
	Shader shader;
	Texture2D texture;
	unsigned int vao;
	// initializes buffer and vertex attributes
	void Init();
	// returns the first Particle index that's currently unused, 
	unsigned int FirstUnusedParticle();
	// respawns particle
	void RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

