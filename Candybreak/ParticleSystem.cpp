#pragma once
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 position, int particleCount)
{
	particleShader = std::make_unique<Shader>("../shader/particle.vert", "../shader/particle.frag", "../shader/particle.geom");
	computeShader = std::make_unique<Shader>(nullptr, nullptr, nullptr, "../shader/particle.comp");

	computeShader->use();

	for (int i = 0; i < 2; i++)
	{
		glGenBuffers(1, &ssbo_pos[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &ssbo_vel[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	}

	glGenBuffers(1, &atomic_counter);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
	GLuint value = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	// Because of a performance warning when reading the atomic counter, we
	// create a buffer to move-to and read-from instead.
	glGenBuffers(1, &temp_buffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL,
		GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> velocities;
	positions.push_back(glm::vec4(position, 1));
	velocities.push_back(glm::vec4(0, 0, 0, 1));
	particle_count = positions.size();

	// copy the data to the SSBO:
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count * sizeof(positions[0]), &positions[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count * sizeof(velocities[0]), &velocities[0]);

	const GLuint position_layout = 0;
	glGenVertexArrays(2, vaos);

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[i]);
		glEnableVertexAttribArray(position_layout);
		glVertexAttribPointer(position_layout, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Unbind VAO
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
	computeShader.reset();
	particleShader.reset();
}

void ParticleSystem::draw(glm::mat4& view, glm::mat4& projection)
{
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);

	particleShader->use();

	particleShader->setMat4("V", view);
	particleShader->setMat4("VP", projection * view);
	particleShader->setVec3("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
	particleShader->setVec3("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

	glBindVertexArray(vaos[0]);
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, particle_count);
	glBindVertexArray(0);
	glUseProgram(0);

	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}