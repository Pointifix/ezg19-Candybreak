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
		float distanceToBoundary = glm::dot(glm::vec3(BOUNDARIES[i]), this->Position) + BOUNDARIES[i].w;

		if (distanceToBoundary < 0) {
			this->Direction = glm::reflect(this->Direction, glm::vec3(BOUNDARIES[i]));
		}
	}
}
