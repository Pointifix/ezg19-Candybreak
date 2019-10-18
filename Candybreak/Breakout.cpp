#include "Breakout.h"

#include <iostream>

Breakout::Breakout(glm::vec3 positionBall, glm::vec3 direction, glm::vec3 positionPad) : ballPosition(positionBall), padPosition(positionPad)
{
	this->Direction = glm::normalize(direction);
}

Breakout::~Breakout()
{

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

			particleSystems.push_back(std::make_unique<ParticleSystem>(this->ballPosition, 8));
		}
	}
}
