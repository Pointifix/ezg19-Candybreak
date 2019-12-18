#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"

class BlurShader
{
public:
	BlurShader();
	~BlurShader();

	GLuint blurredFBO;
	GLuint blurredTexture;

	void blur(GLuint texture, int iterations);
private:
	std::unique_ptr<Shader> shader;

	GLuint pingpongFBO[2];
	GLuint pingpongBuffer[2];
};
