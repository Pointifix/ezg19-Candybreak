#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"
#include "FrameBuffer.h"

const float OFF_SCREEN_RENDER_RATIO = 2.0f;

class VolumetricLightShader
{
public:
	VolumetricLightShader();
	~VolumetricLightShader();

	void use(glm::mat4 view, glm::mat4 projection, GLuint lightDepthMap, GLuint depthmap);
	void finish(GLuint framebuffer);
private:
	std::unique_ptr<Shader> shader;

	int offScreenWidth, offScreenHeight;

	std::unique_ptr<FrameBuffer> framebuffer;
};

