#pragma once
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

const float SPEED = 50.0f;
const float SENSITIVITY = 0.08f;
const float ROLLSENSITIVITY = 50.0f;

class FreeCamera : public Camera
{
public:
	float movementSpeed;
	float mouseSensitivity;
	float rollSensitivity;

	FreeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	~FreeCamera();

	void processInput();
	void update();
	void processMouseMovement(double xoffset, double yoffset);
private:
	void processKeyboard(FreeCameraMovement direction, double deltaTime);
};

