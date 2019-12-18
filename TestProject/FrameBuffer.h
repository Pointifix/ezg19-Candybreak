#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	GLuint FBO;
	GLuint FBOtexture, FBOdepthmap;
};

