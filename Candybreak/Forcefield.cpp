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
	glDepthMask(false);

	forcefieldShader->use();

	forcefieldShader->setMat4("projection", projection);
	forcefieldShader->setMat4("view", view);

	for (unsigned int i = 0; i < 5; i++)
	{
		if (i == 4) forcefieldModel->RotationAxes = glm::vec3(1.0f, 0.0f, 0.0f);
		else forcefieldModel->RotationAxes = glm::vec3(0.0f, 1.0f, 0.0f);

		forcefieldModel->Position = this->positions[i];
		forcefieldModel->Rotation = this->rotations[i];

		auto* hitQueue = &breakout.hitQueues[i];

		int size = hitQueue->size();
		if (size > 4) size = 4;

		forcefieldShader->setInt("hitCount", 0);
		if (size > 0) {

			if (currentFrame - hitQueue->front().z > 3.0f)
			{
				hitQueue->pop();
				size--;				
			}

			forcefieldShader->setInt("hitCount", size);
			int j = 0;

			for (auto it = hitQueue->begin(); j < 4 && it != hitQueue->end(); ++it)
			{
				forcefieldShader->setVec3("hitCoords[" + std::to_string(j) + "]", glm::vec3(it->x, it->y, currentFrame - it->z));
				j++;
			}
		}

		forcefieldModel->Draw(*forcefieldShader);
	}

	glEnable(GL_CULL_FACE);
	glDepthMask(true);
}