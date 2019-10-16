#pragma once
#include "Shader.h"
#include <vector>

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 position, int particleCount);
	~ParticleSystem();

	void update(float deltaTime, glm::mat4& model);
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

	const unsigned int MAX_PARTICLES = 1024;

	unsigned int particle_count;
	unsigned int current_particle_count;
};

