#include "FreeCamera.h"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

FreeCamera::FreeCamera(glm::vec3 position, glm::vec3 rotation) : movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), rollSensitivity(ROLLSENSITIVITY)
{
	this->position = position;

	setYaw(rotation.x);
	setPitch(rotation.y);
	setRoll(rotation.z);
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::update()
{
}

void FreeCamera::processInput()
{
	if (glfwGetKey(global::window, GLFW_KEY_W) == GLFW_PRESS)
		processKeyboard(FORWARD, global::deltaTime);
	if (glfwGetKey(global::window, GLFW_KEY_S) == GLFW_PRESS)
		processKeyboard(BACKWARD, global::deltaTime);
	if (glfwGetKey(global::window, GLFW_KEY_A) == GLFW_PRESS)
		processKeyboard(LEFT, global::deltaTime);
	if (glfwGetKey(global::window, GLFW_KEY_D) == GLFW_PRESS)
		processKeyboard(RIGHT, global::deltaTime);
	
	if (glfwGetKey(global::window, GLFW_KEY_Q) == GLFW_PRESS)
		processKeyboard(ROLLLEFT, global::deltaTime);
	if (glfwGetKey(global::window, GLFW_KEY_E) == GLFW_PRESS)
		processKeyboard(ROLLRIGHT, global::deltaTime);
}

void FreeCamera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	changeYaw(-xoffset);
	changePitch(yoffset);
}

void FreeCamera::processKeyboard(FreeCameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;

	if (direction == ROLLLEFT)
		changeRoll(rollSensitivity * -deltaTime);
	if (direction == ROLLRIGHT)
		changeRoll(rollSensitivity  * deltaTime);
}