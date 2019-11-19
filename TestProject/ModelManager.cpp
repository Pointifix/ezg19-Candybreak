#include "ModelManager.h"



ModelManager::ModelManager()
{
	this->map = std::make_unique<Model>("../assets/TestMapVolumetric/untitled.obj");
	this->tree = std::make_unique<Model>("../assets/Map/tree.obj");
}


ModelManager::~ModelManager()
{
}
