#pragma once
#include "InputHandler.h"

#include "Globals.h"
#include <iomanip>

bool firstMouse = true;
float lastX, lastY;
bool firstPath = true;

void processInput()
{
	if (glfwGetKey(global::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(global::window, true);

	global::camera->processInput();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		glm::vec3 position = global::camera->position;
		glm::vec3 rotation = global::camera->rotation;

		if (firstPath)
		{
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "\t{" << std::endl;
			firstPath = false;
		}

		std::cout << "\t{ glm::vec3(" << position.x << "f, " << position.y << "f, " << position.z << "f), glm::vec3(" << rotation.x << "f, " << rotation.y << "f, " << rotation.z << "f), 0.005 }," << std::endl;
	}
	if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		std::cout << "}," << std::endl << "{" << std::endl;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

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