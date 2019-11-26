#include "Camera.h"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::changeYaw(float angle)
{
	this->rotation.x += angle;

	front = glm::rotateY(front, glm::radians(angle));
	up = glm::rotateY(up, glm::radians(angle));
	right = glm::rotateY(right, glm::radians(angle));
}

void Camera::changePitch(float angle)
{
	this->rotation.y += angle;

	front = glm::rotate(front, glm::radians(angle), right);
	up = glm::rotate(up, glm::radians(angle), right);
}

void Camera::changeRoll(float angle)
{
	this->rotation.z += angle;

	right = glm::rotate(right, glm::radians(angle), front);
	up = glm::rotate(up, glm::radians(angle), front);
}

const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

void Camera::setYaw(float angle)
{
	this->rotation.x = angle;

	front = glm::rotateY(FRONT, glm::radians(angle));
	up = glm::rotateY(UP, glm::radians(angle));
	right = glm::rotateY(RIGHT, glm::radians(angle));
}

void Camera::setPitch(float angle)
{
	this->rotation.y = angle;

	front = glm::rotate(front, glm::radians(angle), RIGHT);
	up = glm::rotate(up, glm::radians(angle), RIGHT);
}

void Camera::setRoll(float angle)
{
	this->rotation.z = angle;

	right = glm::rotate(right, glm::radians(angle), FRONT);
	up = glm::rotate(up, glm::radians(angle), FRONT);
}