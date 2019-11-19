#include "DepthShader.h"
#include "Globals.h"

const int SIZE = 2048;

DepthShader::DepthShader()
{
	shader = std::make_unique<Shader>("../shader/depth.vert", "../shader/depth.frag");

	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SIZE, SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "VOLUMETRIC LIGHT FRAMEBUFFER INITIALIZATION FAILED" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


DepthShader::~DepthShader()
{
}


void DepthShader::use(glm::mat4 view, glm::mat4 projection)
{
	glViewport(0, 0, SIZE, SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader->use();

	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
}


void DepthShader::draw(Model model)
{
	shader->setMat4("model", model.model);

	for (size_t i = 0; i < model.meshes.size(); i++)
	{
		Mesh mesh = model.meshes[i];

		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}


void DepthShader::finish()
{
	glViewport(0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
