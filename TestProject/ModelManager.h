#pragma once
#include "Model.h"
#include "Bricks.h"

class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	std::unique_ptr<Model>map;
	std::unique_ptr<Model>tree;
	std::unique_ptr<Model>brick;
	std::unique_ptr<Model>ball;
	std::unique_ptr<Model>pad;
	std::unique_ptr<Model>plane;
	std::unique_ptr<Model>light;

	std::unique_ptr<Bricks>bricks;

	std::unique_ptr<Mesh> screenQuad;
};

