#include "BloomShader.h"

BloomShader::BloomShader()
{
	shader = std::make_unique<Shader>("../shader/bloom.vert", "../shader/bloom.frag");

	offScreenWidth = setting::SCREEN_WIDTH / OFF_SCREEN_RENDER_RATIO;
	offScreenHeight = setting::SCREEN_HEIGHT / OFF_SCREEN_RENDER_RATIO;

	blurShader = std::make_unique<BlurShader>(offScreenWidth, offScreenHeight);

	framebuffer = std::make_unique<FrameBuffer>(offScreenWidth, offScreenHeight, GL_RGBA16F, GL_FLOAT);
}

BloomShader::~BloomShader()
{
}

void BloomShader::bloom(GLuint texture)
{
	shader->use();

	this->framebuffer->bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setInt("image", 0);

	glBindVertexArray(global::screenQuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	blurShader->blur(this->framebuffer->FBOtexture, 2);

	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}