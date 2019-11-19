#pragma once
#include "Model.h"

class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	std::unique_ptr<Model>map;
	std::unique_ptr<Model>tree;
};

