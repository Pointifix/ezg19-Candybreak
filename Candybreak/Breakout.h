#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <array>

const float BOX_SIZE = 50.0f;

const std::array<glm::vec4, 6> BOUNDARIES = {
	glm::vec4(1.0, 0.0, 0.0, BOX_SIZE / 2.0),
	glm::vec4(-1.0, 0.0, 0.0, BOX_SIZE / 2.0),
	glm::vec4(0.0, 0.0, 1.0, BOX_SIZE / 2.0),
	glm::vec4(0.0, 0.0, -1.0, BOX_SIZE / 2.0),
	glm::vec4(0.0, 1.0, 0.0, BOX_SIZE / 2.0),
	glm::vec4(0.0, -1.0, 0.0, BOX_SIZE / 2.0)
};
const float BALL_SPEED = 32.0f;

class Breakout
{
public:
	glm::vec3 Position;
	glm::vec3 Direction;

	Breakout(glm::vec3 position, glm::vec3 direction);
	~Breakout();

	void update(float deltaTime);

private:
	void boundaryCheck();
};

