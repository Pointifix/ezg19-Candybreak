#include "Breakout.h"

#include <iostream>

Breakout::Breakout(glm::vec3 position, glm::vec3 direction) : Position(glm::vec3(0.0f, 0.0f, 0.0f)), Direction(glm::vec3(0.0f, 0.0f, 1.0f))
{
	this->Position = position;
	this->Direction = glm::normalize(direction);
}

Breakout::~Breakout()
{

}

void Breakout::update(float deltaTime)
{
	this->Position += this->Direction * deltaTime * BALL_SPEED;

	this->boundaryCheck();
}

void Breakout::boundaryCheck()
{
	for (int i = 0; i < BOUNDARIES.size(); i++)
	{
		float distanceToBoundary = glm::dot(glm::vec3(BOUNDARIES[i]), this->Position) + BOUNDARIES[i].w - (BALL_SIZE / 2.0f);

		if (distanceToBoundary < 0 && glm::dot(this->Direction, glm::vec3(BOUNDARIES[i])) < 0) {
			this->Direction = glm::reflect(this->Direction, glm::vec3(BOUNDARIES[i]));

			if (i == 0) this->hitQueues[i].push(glm::vec3((this->Position.x + BOX_SIZE / 2) / BOX_SIZE, (-this->Position.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 1) this->hitQueues[i].push(glm::vec3((-this->Position.z + BOX_SIZE / 2) / BOX_SIZE, (-this->Position.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 2) this->hitQueues[i].push(glm::vec3((-this->Position.x + BOX_SIZE / 2) / BOX_SIZE, (-this->Position.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 3) this->hitQueues[i].push(glm::vec3((this->Position.z + BOX_SIZE / 2) / BOX_SIZE, (-this->Position.y + BOX_SIZE / 2) / BOX_SIZE, currentFrame));
			if (i == 4) this->hitQueues[i].push(glm::vec3((this->Position.x + BOX_SIZE / 2) / BOX_SIZE, (-this->Position.z + BOX_SIZE / 2) / BOX_SIZE, currentFrame));			
		}
	}
}
