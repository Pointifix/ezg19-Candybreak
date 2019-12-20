#include "BlurShader.h"

BlurShader::BlurShader(int width, int height)
{
	shader = std::make_unique<Shader>("../shader/blur.vert", "../shader/blur.frag");

	this->width = width;
	this->height = height;

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
	}
}

BlurShader::~BlurShader()
{
}

void BlurShader::blur(GLuint texture, int iterations, bool bloom)
{
	shader->use();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[true]);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[false]);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bool horizontal = true, first_iteration = true;

	glActiveTexture(GL_TEXTURE0);
	shader->setInt("image", 0);
	for (int i = 0; i < iterations * 2; i++)
	{
		shader->setBool("bloom", bloom && first_iteration);
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		shader->setBool("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? texture : pingpongBuffer[!horizontal]);

		glBindVertexArray(global::screenQuadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}

	glViewport(0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->blurredFBO = pingpongFBO[!horizontal];
	this->blurredTexture = pingpongBuffer[!horizontal];
}
