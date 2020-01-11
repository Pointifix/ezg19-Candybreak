#include "SpotLight.h"



SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutoff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->position = position;
	this->direction = direction;
	this->startDirection = direction;
	
	this->cutOff = cutOff;
	this->outerCutOff = outerCutoff;
	
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

SpotLight::~SpotLight()
{
}
