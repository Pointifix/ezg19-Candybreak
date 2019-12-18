#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Globals.h"

class Bricks
{
public:
	Bricks(GLuint VAO);
	~Bricks();

	GLuint IBO;
	void update();
};