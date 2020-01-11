#pragma once
#include <glm/glm.hpp>

class SpotLight
{
public:
	SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutoff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~SpotLight();

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 startDirection;

	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};