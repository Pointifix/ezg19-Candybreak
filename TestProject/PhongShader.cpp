#include "PhongShader.h"

PhongShader::PhongShader()
{
	shader = std::make_unique<Shader>("../shader/phong.vert", "../shader/phong.frag");

	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &color);
	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "VOLUMETRIC LIGHT FRAMEBUFFER INITIALIZATION FAILED" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


PhongShader::~PhongShader()
{
}

void PhongShader::use(glm::mat4 view, glm::mat4 projection, GLuint depthmap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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

void PhongShader::draw(Model model)
{
	shader->setMat4("model", model.model);
	
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

		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
