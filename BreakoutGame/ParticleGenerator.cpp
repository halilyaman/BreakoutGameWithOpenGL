#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->Init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int new_particles, glm::vec2 offset)
{
	// add new particles
	for (unsigned int i = 0; i < new_particles; i++)
	{
		int unused_particle = this->FirstUnusedParticle();
		this->RespawnParticle(this->particles[unused_particle], object, offset);
	}
	// update all particles
	for (unsigned int i = 0; i < this->amount; i++)
	{
		Particle& p = this->particles[i];
		p.life -= dt; // reduce life
		if (p.life > 0.0f) // particle is alive thus update
		{
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5f;
		}
	}
}

// render all particles
void ParticleGenerator::Draw()
{
	// use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle& particle : this->particles)
	{
		if (particle.life >= 0)
		{
			this->shader.SetVector2f("offset", particle.position);
			this->shader.SetVector4f("color", particle.color);
			this->texture.Bind();
			glBindVertexArray(this->vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	// don't forget to reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Init()
{
	// set up mesh and attribute properties
	unsigned int vbo;
	float particle_quad[] = 
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(this->vao);
	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// create default particle instances
	for (unsigned int i = 0; i < this->amount; i++)
	{
		this->particles.push_back(Particle());
	}
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::FirstUnusedParticle()
{
	// first search from last used particle, this will usually return almost instantly
	for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.position = object.position_ + random + offset;
	particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.life = 1.0f;
	particle.velocity = object.velocity_ * 0.1f;
}