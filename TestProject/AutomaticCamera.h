#pragma once
#include "Globals.h"
#include <glm/gtx/spline.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "Camera.h"

class AutomaticCamera : public Camera
{
public:
	AutomaticCamera();
	~AutomaticCamera();

	void processInput();
	void update();
	void processMouseMovement(float xoffset, float yoffset);
};