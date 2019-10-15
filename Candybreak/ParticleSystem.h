#pragma once
#include "Shader.h"
#include <vector>

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 position, int particleCount);
	~ParticleSystem();

	void draw(glm::mat4& view, glm::mat4& projection);
private:
	std::unique_ptr<Shader> particleShader;
	std::unique_ptr<Shader> computeShader;

	GLuint ssbo_pos[2];
	GLuint ssbo_vel[2];
	GLuint vaos[2];

	GLuint atomic_counter;
	GLuint temp_buffer;

	const int MAX_PARTICLES = 1024;

	int particle_count;
};

