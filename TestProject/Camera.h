#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 position; //x y z
	glm::vec3 rotation; //yaw pitch roll

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::mat4 getViewMatrix();
	virtual void update() = 0;
	virtual void processInput() = 0;
	virtual void processMouseMovement(float xoffset, float yoffset) = 0;
protected:
	void changeYaw(float angle);
	void changePitch(float angle);
	void changeRoll(float angle);

	void setYaw(float angle);
	void setPitch(float angle);
	void setRoll(float angle);
};