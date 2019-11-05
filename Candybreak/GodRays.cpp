#include "GodRays.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Init.h"

extern glm::vec2 screenSize;

GodRays::GodRays()
{

}

GodRays::~GodRays()
{

}

void GodRays::init()
{
	quad = std::make_unique<Model>("../assets/Quad/quad.obj");
	ball = std::make_unique<Model>("../assets/Ball/ball.obj");
	godRaysShader = std::make_unique<Shader>("../shader/godrays.vert", "../shader/godrays.frag");

	int offScreenWidth = screenSize.x / OFF_SCREEN_RENDER_RATIO;
	int offScreenHeight = screenSize.y / OFF_SCREEN_RENDER_RATIO;

	glGenTextures(1, &fboTextureId);
	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, offScreenWidth, offScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a renderbuffer object to store depth info
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, offScreenWidth, offScreenHeight);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextureId, 0);
	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "GOD RAYS FRAMEBUFFER INITIALIZATION FAILED" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GodRays::draw(Model map, glm::mat4 view, glm::mat4 projection)
{
	// first pass
	glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, screenSize.x / OFF_SCREEN_RENDER_RATIO, screenSize.y / OFF_SCREEN_RENDER_RATIO);

	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	godRaysShader->use();

	godRaysShader->setInt("pass", 0);
	godRaysShader->setMat4("view", view);
	godRaysShader->setMat4("projection", projection);

	godRaysShader->setBool("light", false);

	map.Draw(*godRaysShader);

	godRaysShader->setBool("light", true);

	ball->Draw(*godRaysShader);

	

	// second pass
	godRaysShader->setInt("pass", 1);

	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, screenSize.x / OFF_SCREEN_RENDER_RATIO, screenSize.y / OFF_SCREEN_RENDER_RATIO, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboTextureId);

	//glm::mat4 orthoProjection = glm::ortho(-screenSize.x / 2, screenSize.x / 2, -screenSize.y / 2, screenSize.y / 2, 0.1f, 1000.0f);

	//godRaysShader->setMat4("projection", orthoProjection);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(quad->meshes[0].VAO);
	glDrawElements(GL_TRIANGLES, quad->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, screenSize.x, screenSize.y);

	glEnable(GL_DEPTH_TEST);
}
