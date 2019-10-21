#pragma once
#include <iostream>

#include "Model.h"
#include "Breakout.h"
#include "Shader.h"

extern GLuint bricksIBO;

class Bricks
{
public:
	Bricks();
	~Bricks();

	void drawInstanced(Shader shader);
private:
	std::unique_ptr<Model> brickModel;
};