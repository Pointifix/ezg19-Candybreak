#include "Forcefield.h"

Forcefield::Forcefield(int x)
{
	forcefieldShader = std::make_unique<Shader>("../shader/forcefield.vert", "../shader/forcefield.frag");
	forcefieldModel = std::make_unique<Model>("../assets/Forcefield/forcefield.obj");
}

Forcefield::~Forcefield()
{

}

void Forcefield::drawForcefield(glm::mat4& view, glm::mat4& projection)
{
	glDisable(GL_CULL_FACE);

	forcefieldShader->use();

	forcefieldShader->setMat4("projection", projection);
	forcefieldShader->setMat4("view", view);

	std::map<float, int> sorted;
	for (unsigned int i = 0; i < 5; i++)
	{
		float distance = glm::length(camera.Position - this->positions[i]);
		sorted[distance] = i;
	}

	for (std::map<float, int>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		int i = it->second;

		if (i == 4) forcefieldModel->RotationAxes = glm::vec3(1.0f, 0.0f, 0.0f);
		else forcefieldModel->RotationAxes = glm::vec3(0.0f, 1.0f, 0.0f);

		forcefieldModel->Position = this->positions[i];
		forcefieldModel->Rotation = this->rotations[i];

		forcefieldShader->setVec2("hitCoords[0]", glm::vec2(breakout.hits[i * 4].x, breakout.hits[i * 4].y));
		forcefieldShader->setInt("hitCount", 1);
		forcefieldShader->setFloat("time", currentFrame - breakout.hits[i * 4].z);

		forcefieldModel->Draw(*forcefieldShader);
	}
	glEnable(GL_CULL_FACE);
}
