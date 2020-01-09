#include "PhongShader.h"

PhongShader::PhongShader()
{
	shader = std::make_unique<Shader>("../shader/phong.vert", "../shader/phong.frag");

	framebuffer = std::make_unique<FrameBuffer>(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_RGBA16F, GL_FLOAT, false);
}

PhongShader::~PhongShader()
{
}

void PhongShader::use(glm::mat4 view, glm::mat4 projection, GLuint depthmap)
{
	this->framebuffer->bind();

	shader->use();

	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 1);

	shader->setFloat("material.shininess", 32.0f);

	shader->setVec3("directionalLight.direction", global::directionalLight->direction);
	shader->setVec3("directionalLight.ambient", global::directionalLight->ambient);
	shader->setVec3("directionalLight.diffuse", global::directionalLight->diffuse);
	shader->setVec3("directionalLight.specular", global::directionalLight->specular);

	shader->setVec3("viewPos", global::camera->position);

	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	shader->setMat4("lightView", global::directionalLight->view);
	shader->setMat4("lightProjection", global::directionalLight->projection);

	glActiveTexture(GL_TEXTURE2);
	shader->setInt("shadowMap", 2);
	glBindTexture(GL_TEXTURE_2D, depthmap);
}

void PhongShader::draw(Model model, bool light)
{
	shader->setMat4("model", model.model);
	shader->setBool("isALight", light);
	
	for (size_t i = 0; i < model.meshes.size(); i++)
	{
		Mesh mesh = model.meshes[i];

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

			shader->setInt((name + number).c_str(), j);
			glBindTexture(GL_TEXTURE_2D, mesh.textures[j].id);
		}

		shader->setVec4("material.diffuseColor", mesh.material->diffuse);
		shader->setBool("material.diffuseMode", mesh.textures.size() == 0);

		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}


void PhongShader::drawInstanced(Model model, int size)
{
	shader->setMat4("model", model.model);
	shader->setBool("isALight", false);

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < model.meshes[0].textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = model.meshes[0].textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, model.meshes[0].textures[i].id);
	}

	shader->setVec4("material.diffuseColor", model.meshes[0].material->diffuse);
	shader->setBool("material.diffuseMode", model.meshes[0].textures.size() == 0);

	glBindVertexArray(model.meshes[0].VAO);
	glDrawElementsInstanced(GL_TRIANGLES, model.meshes[0].indices.size(), GL_UNSIGNED_INT, 0, size);
	glBindVertexArray(0);
}
