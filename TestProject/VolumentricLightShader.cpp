#include "VolumentricLightShader.h"

VolumetricLightShader::VolumetricLightShader()
{
	shader = std::make_unique<Shader>("../shader/volumetriclight.vert", "../shader/volumetriclight.frag");

	offScreenWidth = setting::SCREEN_WIDTH / OFF_SCREEN_RENDER_RATIO;
	offScreenHeight = setting::SCREEN_HEIGHT / OFF_SCREEN_RENDER_RATIO;

	framebuffer = std::make_unique<FrameBuffer>(offScreenWidth, offScreenHeight);
}

VolumetricLightShader::~VolumetricLightShader()
{
}

void VolumetricLightShader::use(glm::mat4 view, glm::mat4 projection, GLuint lightDepthMap, GLuint depthmap)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer->FBO);

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

	shader->setMat4("viewInv", glm::inverse(view));
	shader->setMat4("projectionInv", glm::inverse(projection));
}

void VolumetricLightShader::finish(GLuint framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glBindVertexArray(global::screenQuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
