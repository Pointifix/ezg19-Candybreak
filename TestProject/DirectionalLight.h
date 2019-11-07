#pragma once
#include <glm/glm.hpp>

class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::mat4 view;
	glm::mat4 projection;
};

