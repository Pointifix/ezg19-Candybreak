#pragma once
#include "Shader.h"
#include <vector>

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 position, int particleCount, glm::vec3 color);
	~ParticleSystem();

	glm::vec3 color;

	unsigned int current_particle_count;

	void update(float deltaTime);
	void draw(glm::mat4& view, glm::mat4& projection);
private:
	std::unique_ptr<Shader> particleShader;
	std::unique_ptr<Shader> computeShader;

	GLuint ssbo_pos[2];
	GLuint ssbo_vel[2];
	GLuint vaos[2];

	unsigned int currentSSBO = 0;

	GLuint atomic_counter;
	GLuint temp_buffer;

	const unsigned int MAX_PARTICLES = 32768;

	unsigned int particle_count;
};

