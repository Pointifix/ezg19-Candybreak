#pragma once
#include <iostream>
#include <glm/glm.hpp>

#include "Breakout.h"
#include "Model.h"
#include "Shader.h"
#include "Globals.h"

class VoidShader
{
public:
	VoidShader();
	~VoidShader();

	void draw(glm::mat4 view, glm::mat4 projection);
private:
	std::unique_ptr<Model>voidModel;
	std::unique_ptr<Shader>voidShader;
};

