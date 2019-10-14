#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>
#include "Init.h"
#include "InputHandler.h"
#include "Shader.h"
#include "FreeCamera.h"
#include "Model.h"
#include "Breakout.h"
#include "Skybox.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace irrklang;

extern GLFWwindow* window;
extern ISoundEngine *SoundEngine;

FreeCamera camera(glm::vec3(0.0f, 0.0f, 100.0f));
Breakout breakout(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 1.0f, 0.5f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float t = 0.0f;

extern unsigned int setting::SCREEN_WIDTH;
extern unsigned int setting::SCREEN_HEIGHT;

int main()
{
	// read settings, init glew, flgw, glad
	// ------------------------------------
	int error = init();
	if (error) return error;

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// build and compile shaders
	// -------------------------
	Shader ourShader("../shader/simple.vert", "../shader/simple.frag");

	// init skybox
	// -----------
	Shader skyboxShader = initSkybox();

	// load models
	// -----------
	Model map("../assets/Map/map.obj");
	Model ballModel("../assets/Ball/ball.obj");

	ourShader.use();
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);

	// init time
	// ---------
	lastFrame = glfwGetTime();

	// start song (last call before loop)
	// ----------------------------------
	ISound *candylandSong = SoundEngine->play2D("../assets/Candyland-Tobu.mp3", false, false, ESM_NO_STREAMING);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// calculate interpolation value t which starts at 0.0 and raises to 1.0 until the song finishes
		if (candylandSong->getPlayPosition() < candylandSong->getPlayLength())
			t = 1.0f - ((float)(candylandSong->getPlayLength() - candylandSong->getPlayPosition()) / candylandSong->getPlayLength());
		else
			t = 1.0f;

		// input
		// -----
		processInput(window);

		// update Breakout
		// ---------------
		breakout.update(deltaTime);

		// render
		// ------
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("material.shininess", 32.0f);

		// directional light
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		ourShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("dirLight.specular", 0.8f, 0.8f, 0.8f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)setting::SCREEN_WIDTH / (float)setting::SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render map
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ourShader.setMat4("model", model);
		map.Draw(ourShader);

		// render ball
		model = glm::mat4(1.0f);
		model = glm::translate(model, breakout.Position);
		ourShader.setMat4("model", model);
		ballModel.Draw(ourShader);

		// draw Skybox (always as last for performance)
		drawSkybox(view, projection, skyboxShader, t);

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