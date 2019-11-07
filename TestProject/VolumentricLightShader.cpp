#include "VolumentricLightShader.h"

VolumetricLightShader::VolumetricLightShader()
{
	shader = std::make_unique<Shader>("../shader/volumetriclight.vert", "../shader/volumetriclight.frag");

	offScreenWidth = setting::SCREEN_WIDTH / OFF_SCREEN_RENDER_RATIO;
	offScreenHeight = setting::SCREEN_HEIGHT / OFF_SCREEN_RENDER_RATIO;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &FBOtexture);
	glBindTexture(GL_TEXTURE_2D, FBOtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, offScreenWidth, offScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &FBOdepthmap);
	glBindTexture(GL_TEXTURE_2D, FBOdepthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, offScreenWidth, offScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBOdepthmap, 0);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "VOLUMETRIC LIGHT FRAMEBUFFER INITIALIZATION FAILED" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	vector<Vertex> vertices = {
		{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};

	vector<unsigned int> indices = {
		0,1,2,
		0,3,1
	};

	vector<Texture> textures;

	screenQuad = std::make_unique<Mesh>(vertices, indices, textures);
}


VolumetricLightShader::~VolumetricLightShader()
{
}


void VolumetricLightShader::use(glm::mat4 view, glm::mat4 projection, GLuint lightDepthMap, GLuint depthmap)
{
	glViewport(0, 0, offScreenWidth, offScreenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();

	glActiveTexture(GL_TEXTURE0);
	shader->setInt("depthMap", 0);
	glBindTexture(GL_TEXTURE_2D, depthmap);

	glActiveTexture(GL_TEXTURE1);
	shader->setInt("lightDepthMap", 1);
	glBindTexture(GL_TEXTURE_2D, lightDepthMap);

	shader->setVec3("directionalLight.position", global::directionalLight->position);

	shader->setVec3("directionalLight.direction", global::directionalLight->direction);
	shader->setVec3("directionalLight.ambient", global::directionalLight->ambient);
	shader->setVec3("directionalLight.diffuse", global::directionalLight->diffuse);
	shader->setVec3("directionalLight.specular", global::directionalLight->specular);

	shader->setVec3("viewPos", global::camera->position);

	shader->setMat4("lightView", global::directionalLight->view);
	shader->setMat4("lightProjection", global::directionalLight->projection);

	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	shader->setMat4("viewInv", glm::inverse(view));
	shader->setMat4("projectionInv", glm::inverse(projection));
}

void VolumetricLightShader::finish()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindVertexArray(screenQuad->VAO);
	glDrawElements(GL_TRIANGLES, screenQuad->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
