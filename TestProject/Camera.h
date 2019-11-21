#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 position;

	virtual glm::mat4 getViewMatrix() = 0;
	virtual void processInput() = 0;
	virtual void processMouseMovement(float xoffset, float yoffset) = 0;
	virtual void yawAutoChange(float angle) = 0;
	virtual void pitchAutoChange(float angle) = 0;
	virtual float getPitch() = 0;
	virtual float getYaw() = 0;

	virtual void calcNewRotation(float xoffset, float yoffset) = 0;
};