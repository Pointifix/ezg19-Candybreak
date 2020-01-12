#include "Breakout.h"
#include <iostream>
#include <array>
#include <mutex>
#include <atomic>

#include "Globals.h"

const float BALL_SIZE = 2.0f; //diameter not radius!
const float PAD_SIZE = 6.0f; //diameter not radius!
const float BORDER = 1.0f; //border (black bits) for pad movement boundary
const float BALL_SPEED = 32.0f;
const float PAD_SPEED = 32.0f;

const float BRICK_SIZEXZ = breakout::BOX_SIZE / breakout::BREAKOUT_SIZE;
const float BRICK_SIZEY = breakout::BOX_SIZE / (breakout::BREAKOUT_SIZE * 2.0f);

glm::vec3 lastBallPosition;

const std::array<glm::vec4, 6> BOUNDARIES = {
	glm::vec4(0.0, 0.0, -1.0, breakout::BOX_SIZE / 2.0), //back
	glm::vec4(1.0, 0.0, 0.0, breakout::BOX_SIZE / 2.0), //right
	glm::vec4(0.0, 0.0, 1.0, breakout::BOX_SIZE / 2.0), //front
	glm::vec4(-1.0, 0.0, 0.0, breakout::BOX_SIZE / 2.0), //left
	glm::vec4(0.0, -1.0, 0.0, breakout::BOX_SIZE / 2.0), //top
	glm::vec4(0.0, 1.0, 0.0, breakout::BOX_SIZE / 2.0) //bottom
};

Breakout::Breakout()
{
	this->ballDirection = glm::normalize(this->ballDirection);

	this->ballPosition = glm::vec3(0.0f, (-breakout::BOX_SIZE + BALL_SIZE) / 2.0, 0.0f);
	lastBallPosition = this->ballPosition;
	breakout::ballPosition = this->ballPosition;
	this->padPosition = glm::vec3(0.0f, -breakout::BOX_SIZE / 2.0, 0.0f);
	breakout::padPosition = this->padPosition;

	breakout::bricksPositionMutex.lock();
	for (int x = 0; x < breakout::BREAKOUT_SIZE; x++)
	{
		for (int y = 0; y < breakout::BREAKOUT_SIZE; y++)
		{
			for (int z = 0; z < breakout::BREAKOUT_SIZE; z++)
			{
				if (breakout::bricksBool[y][x][z])
				{
					breakout::bricksPosition.push_back(glm::vec3((x * BRICK_SIZEXZ) - ((breakout::BOX_SIZE - BRICK_SIZEXZ) / 2.0f), (y * BRICK_SIZEY) + (BRICK_SIZEY / 2.0f), (z * BRICK_SIZEXZ) - ((breakout::BOX_SIZE - BRICK_SIZEXZ) / 2.0f)));
				}
			}
		}
	}
	breakout::bricksPositionMutex.unlock();
}

Breakout::~Breakout()
{
}

void Breakout::update()
{
	lastBallPosition = this->ballPosition;
	this->ballPosition += this->ballDirection * (float)global::deltaTimeLogicEngine * BALL_SPEED;
	
	this->padCheck();
	this->boundaryCheck();
	this->bricksCheck();

	if (this->ballDirection.y < 0) breakout::padPosition = this->padPosition;
	breakout::ballPosition = this->ballPosition;
}

void Breakout::boundaryCheck()
{
	for (int i = 0; i < BOUNDARIES.size(); i++)
	{
		float distanceToBoundary = glm::dot(glm::vec3(BOUNDARIES[i]), this->ballPosition) + BOUNDARIES[i].w - (BALL_SIZE / 2.0f);

		if (distanceToBoundary < 0 && glm::dot(this->ballDirection, glm::vec3(BOUNDARIES[i])) < 0) {
			this->ballDirection = glm::reflect(this->ballDirection, glm::vec3(BOUNDARIES[i]));

			float frame = glfwGetTime();

			breakout::hitQueuesMutex.lock();
			if (i == 0) breakout::hitQueues[i].push(glm::vec3((this->ballPosition.x + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, (-this->ballPosition.y + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, frame));
			if (i == 1) breakout::hitQueues[i].push(glm::vec3((-this->ballPosition.z + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, (-this->ballPosition.y + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, frame));
			if (i == 2) breakout::hitQueues[i].push(glm::vec3((-this->ballPosition.x + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, (-this->ballPosition.y + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, frame));
			if (i == 3) breakout::hitQueues[i].push(glm::vec3((this->ballPosition.z + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, (-this->ballPosition.y + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, frame));
			if (i == 4) breakout::hitQueues[i].push(glm::vec3((this->ballPosition.x + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, (-this->ballPosition.z + breakout::BOX_SIZE / 2) / breakout::BOX_SIZE, frame));
			breakout::hitQueuesMutex.unlock();

			global::particleSystemsToAddMutex.lock();
			if (i < 5) global::particleSystemsToAdd.push_back({this->ballPosition, 32, glm::vec3(3.0, 3.0, 3.0)});
			global::particleSystemsToAddMutex.unlock();
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

		cornerPosition.x = (cornerPosition.x + (breakout::BOX_SIZE / 2)) / BRICK_SIZEXZ;
		cornerPosition.y = (cornerPosition.y + (breakout::BOX_SIZE / 2)) / BRICK_SIZEXZ * 2;
		cornerPosition.z = (cornerPosition.z + (breakout::BOX_SIZE / 2)) / BRICK_SIZEXZ;

		glm::ivec3 indices = glm::ivec3(cornerPosition);
		indices.y -= breakout::BREAKOUT_SIZE;

		glm::ivec3 clampedIndices = glm::clamp(indices, 0, breakout::BREAKOUT_SIZE - 1);

		if (clampedIndices != indices) continue;

		if (breakout::bricksBool[clampedIndices.y][clampedIndices.x][clampedIndices.z] == 1)
		{
			glm::vec3 boxMin = glm::vec3((clampedIndices.x * BRICK_SIZEXZ) - (breakout::BOX_SIZE / 2.0f), (clampedIndices.y * BRICK_SIZEY), (clampedIndices.z * BRICK_SIZEXZ) - (breakout::BOX_SIZE / 2.0f));
			glm::vec3 boxMax = glm::vec3(((clampedIndices.x + 1) * BRICK_SIZEXZ) - (breakout::BOX_SIZE / 2.0f), ((clampedIndices.y + 1) * BRICK_SIZEY), ((clampedIndices.z + 1) * BRICK_SIZEXZ) - (breakout::BOX_SIZE / 2.0f));

			glm::vec3 closestPointToBall = glm::max(boxMin, glm::min(ballPosition, boxMax));
			glm::vec3 difference = closestPointToBall - ballPosition;

			this->ballPosition += difference * (glm::length(difference) - (BALL_SIZE / 2.0f));

			this->ballDirection = glm::reflect(this->ballDirection, difference);
			this->ballDirection = glm::normalize(this->ballDirection);

			this->destroyBrick(clampedIndices);

			return;
		}
	}
}

void Breakout::destroyBrick(glm::ivec3 indices)
{
	breakout::bricksBool[indices.y][indices.x][indices.z] = 0;

	glm::vec3 brickPosition = glm::vec3((indices.x * BRICK_SIZEXZ) - ((breakout::BOX_SIZE - BRICK_SIZEXZ) / 2.0f), (indices.y * BRICK_SIZEY) + (BRICK_SIZEY / 2.0f), (indices.z * BRICK_SIZEXZ) - ((breakout::BOX_SIZE - BRICK_SIZEXZ) / 2.0f));

	breakout::bricksToRemoveMutex.lock();
	breakout::bricksToRemove.push_back(brickPosition);
	breakout::bricksToRemoveMutex.unlock();
	breakout::updateBricks = true;

	global::particleSystemsToAddMutex.lock();
	global::particleSystemsToAdd.push_back({ this->ballPosition, 256, glm::vec3(10.0, 10.0, 10.0) });
	global::particleSystemsToAddMutex.unlock();
}

void Breakout::padCheck()
{
	if (this->ballDirection.y < 0)
	{
		glm::vec3 difference = this->padPosition - this->ballPosition;
		difference.y = 0;

		if (glm::length(difference) > 1.0f * (float)global::deltaTimeLogicEngine * PAD_SPEED) difference = glm::normalize(difference);

		padPosition -= glm::vec3(difference.x, 0.0f, difference.z) * (float)global::deltaTimeLogicEngine * PAD_SPEED;

		if (glm::abs(padPosition.x) > breakout::BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER)
		{
			if (padPosition.x > 0) padPosition.x = breakout::BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER;
			else padPosition.x = -(breakout::BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER);
		}

		if (glm::abs(padPosition.z) > breakout::BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER)
		{
			if (padPosition.z > 0) padPosition.z = breakout::BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER;
			else padPosition.z = -(breakout::BOX_SIZE / 2 - PAD_SIZE / 2 - BORDER);
		}
	}
}