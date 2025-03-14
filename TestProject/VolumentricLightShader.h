#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "BlurShader.h"

class VolumetricLightShader
{
public:
	VolumetricLightShader();
	~VolumetricLightShader();

	const float OFF_SCREEN_RENDER_RATIO = 2.0f;

	std::unique_ptr<FrameBuffer> framebuffer;

	std::unique_ptr<BlurShader> blurShader;

	void perform(glm::mat4 view, glm::mat4 projection, GLuint lightDepthMap, GLuint depthmap);
private:
	std::unique_ptr<Shader> shader;

	int offScreenWidth, offScreenHeight;
};

