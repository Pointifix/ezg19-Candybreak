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
		processKeyboard(FreeCameraMovement::FORWARD, global::deltaTimeRenderEngine);
	if (glfwGetKey(global::window, GLFW_KEY_S) == GLFW_PRESS)
		processKeyboard(FreeCameraMovement::BACKWARD, global::deltaTimeRenderEngine);
	if (glfwGetKey(global::window, GLFW_KEY_A) == GLFW_PRESS)
		processKeyboard(FreeCameraMovement::LEFT, global::deltaTimeRenderEngine);
	if (glfwGetKey(global::window, GLFW_KEY_D) == GLFW_PRESS)
		processKeyboard(FreeCameraMovement::RIGHT, global::deltaTimeRenderEngine);
	
	if (glfwGetKey(global::window, GLFW_KEY_Q) == GLFW_PRESS)
		processKeyboard(FreeCameraMovement::ROLLLEFT, global::deltaTimeRenderEngine);
	if (glfwGetKey(global::window, GLFW_KEY_E) == GLFW_PRESS)
		processKeyboard(FreeCameraMovement::ROLLRIGHT, global::deltaTimeRenderEngine);
}

void FreeCamera::processMouseMovement(double xoffset, double yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	changeYaw(-xoffset);
	changePitch(yoffset);
}

void FreeCamera::processKeyboard(FreeCameraMovement direction, double deltaTime)
{
	float velocity = movementSpeed * (float)deltaTime;
	if (direction == FreeCameraMovement::FORWARD)
		position += front * velocity;
	if (direction == FreeCameraMovement::BACKWARD)
		position -= front * velocity;
	if (direction == FreeCameraMovement::LEFT)
		position -= right * velocity;
	if (direction == FreeCameraMovement::RIGHT)
		position += right * velocity;

	if (direction == FreeCameraMovement::ROLLLEFT)
		changeRoll(rollSensitivity * -deltaTime);
	if (direction == FreeCameraMovement::ROLLRIGHT)
		changeRoll(rollSensitivity  * deltaTime);
}