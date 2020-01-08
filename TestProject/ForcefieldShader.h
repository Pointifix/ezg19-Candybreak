#pragma once
#include <iostream>
#include "Shader.h"
#include "Model.h"
#include "Breakout.h"
#include "Globals.h"

#include <queue>

class ForcefieldShader
{
public:
	ForcefieldShader();
	~ForcefieldShader();

	void drawForcefield(glm::mat4& view, glm::mat4& projection);
private:
	std::unique_ptr<Shader> forcefieldShader;
	std::unique_ptr<Model> forcefieldModel;

	const glm::vec3 positions[5] = {
		glm::vec3(0.0f, 0.0f, 25.0f),
		glm::vec3(-25.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -25.0f),
		glm::vec3(25.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 25.0f, 0.0f)
	};
	const float rotations[5] = {
		0.0f, 90.0f, 180.0f, 270.0f, 90.0f
	};
};

