#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Init.h"
#include "InputHandler.h"
#include "Shader.h"
#include "FreeCamera.h"

extern GLFWwindow* window;

FreeCamera camera(glm::vec3(0.0f, 0.0f, 0.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// read settings, init glew, flgw, glad
	// ------------------------------------
	int error = init();
	if (error) return error;

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader("../assets/simple.vert", "../assets/simple.frag");

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}