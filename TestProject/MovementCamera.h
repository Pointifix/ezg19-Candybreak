#pragma once
#include "Globals.h"
#include <glm/gtx/spline.hpp>
#include <iostream>
#include <vector>

class MovementCamera
{
public:
	MovementCamera();
	~MovementCamera();
	void startAutoCamera();
	glm::vec3 cubic_spline(const std::vector<glm::vec3>& cp, float t);
	void init();
	std::vector<std::vector< float >>* camArray;
};

