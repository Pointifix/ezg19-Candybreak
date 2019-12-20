#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"

class BlurShader
{
public:
	BlurShader(int width, int height);
	~BlurShader();

	GLuint blurredFBO;
	GLuint blurredTexture;

	void blur(GLuint texture, int iterations, bool bloom = false);
private:
	std::unique_ptr<Shader> shader;

	GLuint pingpongFBO[2];
	GLuint pingpongBuffer[2];

	int width, height;
};
