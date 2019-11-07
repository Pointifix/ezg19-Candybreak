#include "ModelManager.h"



ModelManager::ModelManager()
{
	this->map = std::make_unique<Model>("../assets/TestMapVolumetric/untitled.obj");
}


ModelManager::~ModelManager()
{
}
