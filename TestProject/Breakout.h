#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Breakout
{
public:
	Breakout();
	~Breakout();
	void update();

private:
	glm::vec3 ballPosition;
	glm::vec3 ballDirection = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 padPosition;

	void boundaryCheck();
	void bricksCheck();
	void destroyBrick(glm::ivec3 indices);
	void padCheck();
};

