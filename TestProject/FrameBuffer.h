#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height, int internalFormat = GL_RGBA, int format = GL_UNSIGNED_BYTE, bool msaa = false);
	~FrameBuffer();

	GLuint FBO;
	GLuint FBOtexture, FBOdepthmap;

	int width, height;
	void bind();
};

