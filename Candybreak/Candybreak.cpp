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
#include "Bricks.h"
#include "Breakout.h"
#include "Skybox.h"
#include "ParticleSystem.h"
#include "Forcefield.h"
#include "Void.h"
#include "GodRays.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace irrklang;

extern GLFWwindow* window;
extern ISoundEngine *SoundEngine;

extern const float BOX_SIZE;

FreeCamera camera(glm::vec3(0.0f, 0.0f, BOX_SIZE * 2));
//Breakout breakout(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.6f, 1.0f));
Breakout breakout(glm::vec3(0.0f, -20.0f, 0.0f), glm::vec3(0.4f, 0.6f, 1.0f));

std::vector<std::unique_ptr<ParticleSystem>> particleSystems;

float startTime = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;

bool paused = true;

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile shaders
	// -------------------------
	Shader simpleShader("../shader/simple.vert", "../shader/simple.frag");

	// init skybox
	// -----------
	initSkybox();

	// init breakout
	// -------------
	breakout.init();

	// init bricks
	// -----------
	Bricks bricks;

	// init void
	// ---------
	Void voidObject;

	// load map
	// -----------
	Model map("../assets/Map/map.obj");

	simpleShader.use();
	simpleShader.setInt("material.diffuse", 0);
	simpleShader.setInt("material.specular", 1);

	Forcefield forceField(6);

	// init GodRays
	// ------------
	GodRays godRays;
	godRays.init();

	// start song
	// ----------
	ISound *candylandSong = SoundEngine->play2D("../assets/Candyland-Tobu.mp3", false, false, ESM_NO_STREAMING);

	// init time
	// ---------
	lastFrame = glfwGetTime();
	startTime = lastFrame;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		currentFrame = glfwGetTime();
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
		if (!paused)
		{
			breakout.update(deltaTime);
		}

		// render
		// ------
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// calculate projection and view matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)setting::SCREEN_WIDTH / (float)setting::SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// update and draw particle system
		// -------------------------------
		for (int i = 0; i < particleSystems.size(); i++)
		{
			if (particleSystems[i]->current_particle_count == 0)
			{
				particleSystems.erase(particleSystems.begin() + i, particleSystems.begin() + i + 1);
				i--;
			}
			else
			{
				if (!paused) particleSystems[i]->update(deltaTime);
				particleSystems[i]->draw(view, projection);
			}
		}
		// draw objects with custom shaders
		voidObject.draw(view, projection);

		// draw objects with lighting
		simpleShader.use();

		simpleShader.setMat4("view", view);
		simpleShader.setMat4("projection", projection);

		simpleShader.setVec3("viewPos", camera.Position);
		simpleShader.setFloat("material.shininess", 32.0f);

		// directional light
		simpleShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		simpleShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		simpleShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		simpleShader.setVec3("dirLight.specular", 0.8f, 0.8f, 0.8f);

		map.Draw(simpleShader);

		breakout.draw(simpleShader, view, projection);

		//TODO draw scene via glMultiDrawElements() instead of glDrawElements()

		// draw Skybox (always after all opaque objects for performance)
		drawSkybox(view, projection, t);

		// draw transparent objects at last (also after skybox)
		forceField.drawForcefield(view, projection);

		simpleShader.use();

		simpleShader.setMat4("view", view);
		simpleShader.setMat4("projection", projection);

		// render bricks
		bricks.draw(simpleShader);

		// render map with god rays
		godRays.draw(map, view, projection);

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