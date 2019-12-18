#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"
#include "FrameBuffer.h"


class CombineShader
{
public:
	CombineShader();
	~CombineShader();

	void combine(GLuint texture1, GLuint texture2, GLuint framebuffer);
private:
	std::unique_ptr<Shader> shader;
};