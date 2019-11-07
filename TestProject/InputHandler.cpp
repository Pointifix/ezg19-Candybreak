#pragma once
#include "InputHandler.h"

#include "Globals.h"

bool firstMouse = true;
float lastX, lastY;

void processInput()
{
	if (glfwGetKey(global::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(global::window, true);

	global::camera->processInput();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	global::camera->processMouseMovement(xoffset, yoffset);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	setting::SCREEN_WIDTH = width;
	setting::SCREEN_HEIGHT = height;
}