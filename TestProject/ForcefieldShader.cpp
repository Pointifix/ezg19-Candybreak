#include "ForcefieldShader.h"

ForcefieldShader::ForcefieldShader()
{
	forcefieldShader = std::make_unique<Shader>("../shader/forcefield.vert", "../shader/forcefield.frag");
	forcefieldModel = std::make_unique<Model>("../assets/Forcefield/forcefield.obj");
}

ForcefieldShader::~ForcefieldShader()
{

}

void ForcefieldShader::drawForcefield(glm::mat4& view, glm::mat4& projection)
{
	glDisable(GL_CULL_FACE);
	glDepthMask(false);

	forcefieldShader->use();

	forcefieldShader->setMat4("projection", projection);
	forcefieldShader->setMat4("view", view);

	for (unsigned int i = 0; i < 5; i++)
	{
		glm::vec3 rotationAxes;
		if (i == 4) rotationAxes = glm::vec3(1.0f, 0.0f, 0.0f);
		else rotationAxes = glm::vec3(0.0f, 1.0f, 0.0f);

		forcefieldModel->model = glm::rotate(glm::translate(glm::mat4(1.0), this->positions[i]), glm::radians(this->rotations[i]), rotationAxes);

		breakout::hitQueuesMutex.lock();
		auto* hitQueue = &breakout::hitQueues[i];

		int size = hitQueue->size();
		if (size > 4) size = 4;

		forcefieldShader->setInt("hitCount", 0);
		if (size > 0) {

			if (global::currentFrameLogicEngine - hitQueue->front().z > 3.0f)
			{
				hitQueue->pop();
				size--;
			}

			forcefieldShader->setInt("hitCount", size);
			int j = 0;

			for (auto it = hitQueue->begin(); j < 4 && it != hitQueue->end(); ++it)
			{
				forcefieldShader->setVec3("hitCoords[" + std::to_string(j) + "]", glm::vec3(it->x, it->y, global::currentFrameLogicEngine - it->z));
				j++;
			}
		}
		breakout::hitQueuesMutex.unlock();

		forcefieldShader->use();
		forcefieldShader->setMat4("model", forcefieldModel->model);

		for (size_t i = 0; i < forcefieldModel->meshes.size(); i++)
		{
			Mesh mesh = forcefieldModel->meshes[i];

			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int j = 0; j < mesh.textures.size(); j++)
			{
				glActiveTexture(GL_TEXTURE0 + j);

				string number;
				string name = mesh.textures[j].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

				forcefieldShader->setInt((name + number).c_str(), j);
				glBindTexture(GL_TEXTURE_2D, mesh.textures[j].id);
			}

			forcefieldShader->setVec4("material.diffuseColor", mesh.material->diffuse);
			forcefieldShader->setBool("material.diffuseMode", mesh.textures.size() == 0);

			glBindVertexArray(mesh.VAO);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	glEnable(GL_CULL_FACE);
	glDepthMask(true);
}