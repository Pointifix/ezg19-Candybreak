#pragma once
#include "Breakout.h"
#include "BrickPositions.h"

#include <iostream>
#include <set>
#include <algorithm>

GLuint bricksIBO;

extern std::array<std::array<std::array<bool, SUBDIVISION>, SUBDIVISION>, SUBDIVISION>boolBrickPositions;

Breakout::Breakout(glm::vec3 positionBall, glm::vec3 direction, glm::vec3 positionPad) : ballPosition(positionBall), padPosition(positionPad)
{
	this->Direction = glm::normalize(direction);

	for (int x = 0; x < SUBDIVISION; x++)
	{
		for (int y = 0; y < SUBDIVISION; y++)
		{
			for (int z = 0; z < SUBDIVISION; z++)
			{
				if (boolBrickPositions[y][x][z])
				{
					bricks.push_back(glm::vec3((x * BRICK_SIZEXZ) - ((BOX_SIZE - BRICK_SIZEXZ) / 2.0f), (y * BRICK_SIZEY) + (BRICK_SIZEY / 2.0f), (z * BRICK_SIZEXZ) - ((BOX_SIZE - BRICK_SIZEXZ) / 2.0f)));
				}
			}
		}
	}
}

Breakout::~Breakout()
{

}

void Breakout::init()
{
	ballModel = std::make_unique<Model>("../assets/Ball/ball.obj");
	padModel = std::make_unique<Model>("../assets/Pad/pad.obj");
}

void Breakout::draw(Shader shader, glm::mat4 view, glm::mat4 projection) {	
	// render ball
	ballModel->Position = this->ballPosition;
	ballModel->Draw(shader);

	// render pads
	padModel->Position = this->padPosition;
	padModel->Draw(shader);
}

void Breakout::update(float deltaTime)
{
	if (this->Direction.y < 0)
	{
		glm::vec3 difference = padPosition - ballPosition;
		difference.y = 0;

		if (glm::length(difference) > 1.0f) difference = glm::normalize(difference);

		padPosition -= glm::vec3(difference.x, 0.0f, difference.z) * deltaTime * PAD_SPEED;

		if (glm::abs(padPosition.x) > BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER)
		{
			if (padPosition.x > 0) padPosition.x = BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER;
			else padPosition.x = -(BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER);
		}

		if (glm::abs(padPosition.z) > BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER)
		{
			if (padPosition.z > 0) padPosition.z = BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER;
			else padPosition.z = -(BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER);
		}
	}

	this->ballPosition += this->Direction * deltaTime * BALL_SPEED;

	this->boundaryCheck();
	this->bricksCheck();
}

void Breakout::boundaryCheck()
{
	for (int i = 0; i < BOUNDARIES.size(); i++)
	{
		float distanceToBoundary = glm::dot(glm::vec3(BOUNDARIES[i]), this->ballPosition) + BOUNDARIES[i].w - (BALL_SIZE / 2.0f);

		if (distanceToBoundary < 0 && glm::dot(this->Direction, glm::vec3(BOUNDARIES[i])) < 0) {
			this->Direction = glm::reflect(this->Direction, glm::vec3(BOUNDARIES[i]));

			if (i == 0) this->hitQueues[i].push(glm::vec3((this->ballPosition.x + BOX_SIZE / 2) / BOX_SIZE, (-this->ballPosition.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 1) this->hitQueues[i].push(glm::vec3((-this->ballPosition.z + BOX_SIZE / 2) / BOX_SIZE, (-this->ballPosition.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 2) this->hitQueues[i].push(glm::vec3((-this->ballPosition.x + BOX_SIZE / 2) / BOX_SIZE, (-this->ballPosition.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 3) this->hitQueues[i].push(glm::vec3((this->ballPosition.z + BOX_SIZE / 2) / BOX_SIZE, (-this->ballPosition.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 4) this->hitQueues[i].push(glm::vec3((this->ballPosition.x + BOX_SIZE / 2) / BOX_SIZE, (-this->ballPosition.z + BOX_SIZE / 2) / BOX_SIZE, currentFrame));

			if (i < 5) particleSystems.push_back(std::make_unique<ParticleSystem>(this->ballPosition, 8, glm::vec3(1.0, 1.0, 1.0)));
		}
	}
}

void Breakout::bricksCheck()
{
	glm::vec3 cornerDelta = glm::vec3(BALL_SIZE / 2.0f);

	glm::vec3 cornerMin = ballPosition - cornerDelta;
	glm::vec3 cornerMax = ballPosition + cornerDelta;

	for (int i = 0; i < 8; i++)
	{
		glm::vec3 signs = glm::vec3((i % 2) * 2 - 1, ((i >> 1 & 1) % 2) * 2 - 1, ((i >> 2 & 1) % 2) * 2 - 1);

		glm::vec3 cornerPosition = ballPosition + (cornerDelta * signs);

		cornerPosition.x = (cornerPosition.x + (BOX_SIZE / 2)) / BRICK_SIZEXZ;
		cornerPosition.y = (cornerPosition.y + (BOX_SIZE / 2)) / BRICK_SIZEXZ * 2;
		cornerPosition.z = (cornerPosition.z + (BOX_SIZE / 2)) / BRICK_SIZEXZ;

		glm::ivec3 indices = glm::ivec3(cornerPosition);
		indices.y -= SUBDIVISION;
		
		glm::ivec3 clampedIndices = glm::clamp(indices, 0, SUBDIVISION - 1);

		if (clampedIndices != indices) continue;

		if (boolBrickPositions[clampedIndices.y][clampedIndices.x][clampedIndices.z] == 1)
		{
			glm::vec3 boxMin = glm::vec3((clampedIndices.x * BRICK_SIZEXZ) - (BOX_SIZE / 2.0f), (clampedIndices.y * BRICK_SIZEY), (clampedIndices.z * BRICK_SIZEXZ) - (BOX_SIZE / 2.0f));
			glm::vec3 boxMax = glm::vec3(((clampedIndices.x + 1) * BRICK_SIZEXZ) - (BOX_SIZE / 2.0f), ((clampedIndices.y + 1) * BRICK_SIZEY), ((clampedIndices.z + 1) * BRICK_SIZEXZ) - (BOX_SIZE / 2.0f));

			glm::vec3 closestPointToBall = glm::max(boxMin, glm::min(ballPosition, boxMax));
			glm::vec3 difference = closestPointToBall - ballPosition;

			this->ballPosition += difference * ((BALL_SIZE / 2.0f) - glm::length(difference));

			this->Direction = glm::reflect(this->Direction, difference);
			this->Direction = glm::normalize(this->Direction);

			this->destroyBrick(clampedIndices);

			return;
		}
	}
}

void Breakout::destroyBrick(glm::ivec3 indices)
{
	boolBrickPositions[indices.y][indices.x][indices.z] = 0;

	glm::vec3 brickPosition = glm::vec3((indices.x * BRICK_SIZEXZ) - ((BOX_SIZE - BRICK_SIZEXZ) / 2.0f), (indices.y * BRICK_SIZEY) + (BRICK_SIZEY / 2.0f), (indices.z * BRICK_SIZEXZ) - ((BOX_SIZE - BRICK_SIZEXZ) / 2.0f));

	bricks.erase(std::remove(bricks.begin(), bricks.end(), brickPosition), bricks.end());

	particleSystems.push_back(std::make_unique<ParticleSystem>(brickPosition, 256, glm::vec3(0.0, 1.0, 0.0)));

	glBindBuffer(GL_ARRAY_BUFFER, bricksIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * bricks.size(), bricks.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}