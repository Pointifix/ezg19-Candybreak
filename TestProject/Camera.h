#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 position; //x y z
	glm::vec3 rotation; //yaw pitch roll

	glm::vec3 up = UP;
	glm::vec3 front = FRONT;
	glm::vec3 right = RIGHT;

	glm::mat4 getViewMatrix();
	virtual void update() = 0;
	virtual void processInput() = 0;
	virtual void processMouseMovement(double xoffset, double yoffset) = 0;
protected:
	void changeYaw(float angle);
	void changePitch(float angle);
	void changeRoll(float angle);

	void setYaw(float angle);
	void setPitch(float angle);
	void setRoll(float angle);
};