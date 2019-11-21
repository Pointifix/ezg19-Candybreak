#include "FreeCamera.h"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

FreeCamera::FreeCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float roll) : front(glm::vec3(0.0f, 0.0f, -1.0f)), right(glm::vec3(1.0f, 0.0f, 0.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), rollSensitivity(ROLLSENSITIVITY)
{
	this->position = position;
	this->up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;

	yawChange(yaw);
	pitchChange(pitch);
	rollChange(roll);

	debugCount = 0;
}

FreeCamera::~FreeCamera()
{
}

glm::mat4 FreeCamera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
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
	if (glfwGetKey(global::window, GLFW_KEY_SPACE) == GLFW_PRESS)
		getPositionCamera();

}

void FreeCamera::processKeyboard(FreeCameraMovement direction, float deltaTime)
{
	debugCount = 0;
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
		rollChange(rollSensitivity * -deltaTime);
	if (direction == ROLLRIGHT)
		rollChange(rollSensitivity  * deltaTime);


}

void FreeCamera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yawChange(-xoffset);
	pitchChange(yoffset);
}

void FreeCamera::yawChange(float angle)
{

	this->yaw += angle;


	//std::cout << "YAW: " << this->yaw << " ROLL: " << front.y << " PITCH: " << this->pitch << std::endl;

	front = glm::rotateY(front, glm::radians(angle));
	up = glm::rotateY(up, glm::radians(angle));
	right = glm::rotateY(right, glm::radians(angle));

	//std::cout << "FRONT: " << front.x << " ROLL: " << front.y << " PITCH: " << front.z << std::endl;

}

void FreeCamera::pitchChange(float angle)
{
	this->pitch += angle;

	front = glm::rotate(front, glm::radians(angle), right);
	up = glm::rotate(up, glm::radians(angle), right);
}

void FreeCamera::rollChange(float angle)
{
	this->roll += angle;

	right = glm::rotate(right, glm::radians(angle), front);
	up = glm::rotate(up, glm::radians(angle), front);
}


void FreeCamera::getPositionCamera() {

	if (debugCount < 1) {
		std::cout << "Position: " << global::camera->position.x << " " << global::camera->position.y << " " << global::camera->position.z << std::endl;
		std::cout << "YAW: " << yaw << " ROLL: " << roll << " PITCH: " << pitch << std::endl;
		std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << std::endl;
		debugCount = 1;
	}
}

void FreeCamera::calcNewRotation(float xoffset, float yoffset) {


		//xoffset *= mouseSensitivity;
	//	yoffset *= mouseSensitivity;



		//std::cout << "YAW: " << glm::radians(yaw) << " Offset: " << glm::radians(xoffset) << std::endl;

		yawAutoChange(-xoffset);
		pitchAutoChange(yoffset);
	
}

void FreeCamera::yawAutoChange(float angle) {


	this->yaw += angle;

	front = glm::rotateY(front, glm::radians(angle));
	up = glm::rotateY(up, glm::radians(angle));
	right = glm::rotateY(right, glm::radians(angle));

}


void FreeCamera::pitchAutoChange(float angle) {

	this->pitch += angle;

	front = glm::rotate(front, glm::radians(angle), right);
	up = glm::rotate(up, glm::radians(angle), right);
}

float FreeCamera::getPitch() {
	return pitch;
}

float FreeCamera::getYaw() {
	return yaw;
}