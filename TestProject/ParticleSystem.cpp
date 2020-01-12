#pragma once
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 position, int particleCount, glm::vec3 color, int mode, glm::vec3 startDirection)
{
	this->particle_count = particleCount;
	this->color = color;
	this->mode = mode;

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

		glGenBuffers(1, &ssbo_rot[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_rot[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &ssbo_col[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_col[i]);
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
	std::vector<glm::vec4> rotations;

	float speed = 10.0f;
	float maxTTL = 1.5f;

	if (mode == 1)
	{
		speed = 9.0;
		maxTTL = 6.0f;
	}

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.5, 0.2);

	for (int i = 0; i < this->particle_count; i++) {
		if (mode == 0)
		{
			positions.push_back(glm::vec4(position, static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * maxTTL));

			rotations.push_back(glm::vec4(
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1));

			glm::vec3 velocity(
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0 - 1.0,
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0 - 1.0,
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0 - 1.0);

			if(glm::length(velocity) > 1.0f) velocity = glm::normalize(velocity);

			velocity *= speed;

			velocities.push_back(glm::vec4(velocity, 1));
		}
		if (mode == 1)
		{
			positions.push_back(glm::vec4(position, static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * maxTTL));

			rotations.push_back(glm::vec4(
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1));

			glm::vec3 velocity(
				static_cast <float> (distribution(generator)) * 2.0 - 1.0,
				static_cast <float> (distribution(generator)) * 2.0 - 1.0,
				static_cast <float> (distribution(generator)) * 2.0 - 1.0);

			if (glm::length(velocity) > 1.0f) velocity = glm::normalize(velocity);

			velocity *= speed;

			velocity += startDirection;

			velocities.push_back(glm::vec4(velocity, static_cast <float> (distribution(generator))));
		}
	}

	current_particle_count = positions.size();

	// copy the data to the SSBO:
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, current_particle_count * sizeof(positions[0]), &positions[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, current_particle_count * sizeof(velocities[0]), &velocities[0]);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_rot[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, current_particle_count * sizeof(rotations[0]), &rotations[0]);

	const GLuint position_layout = 0;
	const GLuint rotation_layout = 1;
	glGenVertexArrays(2, vaos);

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos[i]);
		glEnableVertexAttribArray(position_layout);
		glVertexAttribPointer(position_layout, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_rot[i]);
		glEnableVertexAttribArray(rotation_layout);
		glVertexAttribPointer(rotation_layout, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_col[i]);
	}

	// Unbind VAO
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
	computeShader.reset();
	particleShader.reset();
}

void ParticleSystem::update(float deltaTime)
{
	computeShader->use();

	computeShader->setInt("mode", this->mode);
	computeShader->setFloat("deltaTime", deltaTime);
	computeShader->setUnsignedInt("particleCount", current_particle_count);
	computeShader->setUnsignedInt("maxParticles", MAX_PARTICLES);

	// set SSBO and atomic counters
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_pos[currentSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_vel[currentSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_rot[currentSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_pos[!currentSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_vel[!currentSSBO]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo_rot[!currentSSBO]);

	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 6, atomic_counter);

	currentSSBO = !currentSSBO;
	// Execute compute shader with a 16 x 16 work group size
	GLuint groups = (current_particle_count / (16 * 16)) + 1;
	glDispatchCompute(groups, 1, 1);

	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	// Read atomic counter through a temporary buffer
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer);
	// from atomic counter to temp buffer:
	glCopyBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));
	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	current_particle_count = counterValue[0];

	counterValue[0] = 0;
	glUnmapBuffer(GL_COPY_WRITE_BUFFER);

	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 0, 0, sizeof(GLuint));
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void ParticleSystem::draw(glm::mat4& view, glm::mat4& projection)
{
	glDisable(GL_CULL_FACE);

	particleShader->use();

	particleShader->setMat4("V", view);
	particleShader->setMat4("VP", projection * view);
	particleShader->setVec3("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
	particleShader->setVec3("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

	particleShader->setVec3("systemColor", color);
	particleShader->setInt("mode", this->mode);

	glBindVertexArray(vaos[currentSSBO]);
	glDrawArrays(GL_POINTS, 0, current_particle_count);
	glBindVertexArray(0);
	glUseProgram(0);
	glEnable(GL_CULL_FACE);
}