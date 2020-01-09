#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "BlurShader.h"

class BloomShader
{
public:
	BloomShader();
	~BloomShader();

	const float OFF_SCREEN_RENDER_RATIO = 1.5f;

	std::unique_ptr<FrameBuffer> framebuffer;

	std::unique_ptr<BlurShader> blurShader;

	void bloom(GLuint texture);
private:
	std::unique_ptr<Shader> shader;

	int offScreenWidth, offScreenHeight;
};