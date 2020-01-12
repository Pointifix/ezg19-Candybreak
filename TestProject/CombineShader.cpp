#include "CombineShader.h"

CombineShader::CombineShader()
{
	shader = std::make_unique<Shader>("../shader/combine.vert", "../shader/combine.frag");
}

CombineShader::~CombineShader()
{
}

void CombineShader::combine(GLuint texture1, GLuint texture2, GLuint framebuffer, int mode)
{
	shader->use();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	shader->setInt("mode", mode);
	shader->setFloat("currentTransitionGrayness", global::currentTransitionGrayness);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	shader->setInt("texture1", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	shader->setInt("texture2", 1);

	glBindVertexArray(global::screenQuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}