#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <queue>
#include <array>

#include "ParticleSystem.h"

const float BOX_SIZE = 50.0f;
const float BALL_SIZE = 2.0f; //diameter not radius!
const float PAD_SIZE = 6.0f; //diameter not radius!
const float BORDER = 1.0f; //border (black bits)
const float BALL_SPEED = 32.0f;
const float PAD_SPEED = 32.0f;

extern float currentFrame;

const std::array<glm::vec4, 6> BOUNDARIES = {
	glm::vec4(0.0, 0.0, -1.0, BOX_SIZE / 2.0), //back
	glm::vec4(1.0, 0.0, 0.0, BOX_SIZE / 2.0), //right
	glm::vec4(0.0, 0.0, 1.0, BOX_SIZE / 2.0), //front
	glm::vec4(-1.0, 0.0, 0.0, BOX_SIZE / 2.0), //left
	glm::vec4(0.0, -1.0, 0.0, BOX_SIZE / 2.0), //top
	glm::vec4(0.0, 1.0, 0.0, BOX_SIZE / 2.0) //bottom
};

extern std::vector<std::unique_ptr<ParticleSystem>> particleSystems;

template<typename T, typename Container = std::deque<T> >
class iterable_queue : public std::queue<T, Container>
{
public:
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
	const_iterator begin() const { return this->c.begin(); }
	const_iterator end() const { return this->c.end(); }
};

class Breakout
{
public:
	glm::vec3 ballPosition;
	glm::vec3 Direction;

	glm::vec3 padPosition;

	iterable_queue<glm::vec3> hitQueues[5];

	Breakout(glm::vec3 positionBall = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 positionPad = glm::vec3(0.0f, -BOX_SIZE / 2, 0.0f));
	~Breakout();

	void update(float deltaTime);

private:
	void boundaryCheck();
};

