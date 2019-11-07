#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Globals.h"
#include "Camera.h"

enum FreeCameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	ROLLLEFT,
	ROLLRIGHT
};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 50.0f;
const float SENSITIVITY = 0.08f;
const float ROLLSENSITIVITY = 50.0f;

class FreeCamera : public Camera
{
public:
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float roll;

	float movementSpeed;
	float mouseSensitivity;
	float rollSensitivity;

	FreeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL);
	~FreeCamera();

	glm::mat4 getViewMatrix();

	void processInput();

	void processKeyboard(FreeCameraMovement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset);
private:
	void yawChange(float angle);
	void pitchChange(float angle);
	void rollChange(float angle);
};

