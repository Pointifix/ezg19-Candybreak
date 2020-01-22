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

void Camera::setYaw(float angle)
{
	changeYaw(angle - this->rotation.x);
}

void Camera::setPitch(float angle)
{
	changePitch(angle - this->rotation.y);
}

void Camera::setRoll(float angle)
{
	changeRoll(angle - this->rotation.z);
}