#pragma once
#include "RenderEngine.h"

#include <iostream>
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Globals.h"
#include "Camera.h"
#include "FreeCamera.h"
#include "InputHandler.h"
#include "VolumentricLightShader.h"
#include "PhongShader.h"
#include "DepthShader.h"
#include "Skybox.h"
#include "ModelManager.h"
#include "MovementCamera.h"

std::unique_ptr<VolumetricLightShader> volumetricLightShader;
std::unique_ptr<PhongShader> phongShader;
std::unique_ptr<DepthShader> depthShader;

std::unique_ptr<ModelManager> modelManager;

MovementCamera mvcam;

RenderEngine::RenderEngine()
{
}


RenderEngine::~RenderEngine()
{
}


void RenderEngine::run(std::promise<int> && error)
{
	error.set_value(0);

	if (init())
	{
		error.set_value(-1);
		return;
	}

	while (!glfwWindowShouldClose(global::window))
	{
		update();
		render();
	}

	glfwTerminate();
}

void RenderEngine::update()
{
	currentFrame = glfwGetTime();
	global::deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	global::t = 1.0f - ((float)(global::candylandSong->getPlayLength() - global::candylandSong->getPlayPosition()) / global::candylandSong->getPlayLength());







	//update light position
	/*
	global::directionalLight->direction = glm::rotateX(glm::vec3(0.5f, -0.5f, -0.5f), glm::radians(-360.0f * global::t));
	global::directionalLight->position = global::directionalLight->direction * (-500.0f);
	global::directionalLight->view = glm::lookAt(global::directionalLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	*/

	processInput();
}

void RenderEngine::render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = global::camera->getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)setting::SCREEN_WIDTH / (float)setting::SCREEN_HEIGHT, 0.1f, 500.0f);



	mvcam.startAutoCamera();


	depthShader->use(global::directionalLight->view, global::directionalLight->projection);
	depthShader->draw(*modelManager->map);
	depthShader->draw(*modelManager->tree);
	depthShader->finish();

	phongShader->use(view, projection, depthShader->depthmap);
	//phongShader->draw(*modelManager->map);
	phongShader->draw(*modelManager->tree);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, phongShader->FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, 0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, 0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	drawSkybox(view, projection);

	volumetricLightShader->use(view, projection, depthShader->depthmap, phongShader->depthmap);
	volumetricLightShader->finish();

	glfwSwapBuffers(global::window);
	glfwPollEvents();
}

int RenderEngine::init()
{
	global::camera = new FreeCamera(glm::vec3(0.0f, 0.0f, 100.0f));


	mvcam = MovementCamera();
	mvcam.init();

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	if (setting::FULLSCREEN)
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		global::window = glfwCreateWindow(1980, 1080, setting::TITLE.c_str(), primary, nullptr);
	}
	else
	{
		global::window = glfwCreateWindow(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, setting::TITLE.c_str(), nullptr, nullptr);
	}

	if (global::window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(global::window);
	glfwSetFramebufferSizeCallback(global::window, framebufferSizeCallback);
	glfwSetKeyCallback(global::window, keyCallback);
	glfwSetCursorPosCallback(global::window, mouseCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell GLFW to capture our mouse
	glfwSetInputMode(global::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// enable or disable console (presentation purpose)
	if (setting::CONSOLE)
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	else
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	// enable or disable music
	global::SoundEngine = createIrrKlangDevice();

	if (!setting::MUSIC)
		global::SoundEngine->setSoundVolume(0.0f);

	global::candylandSong = global::SoundEngine->play2D("../assets/Candyland-Tobu.mp3", false, false, ESM_NO_STREAMING);

	lastFrame = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initSkybox();
	
	global::directionalLight = std::make_unique<DirectionalLight>();
	global::directionalLight->direction = glm::vec3(0.5f, -0.5f, -0.5f);
	global::directionalLight->position = global::directionalLight->direction * (-500.0f);
	global::directionalLight->ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	global::directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	global::directionalLight->specular = glm::vec3(0.8f, 0.8f, 0.8f);

	global::directionalLight->projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
	global::directionalLight->view = glm::lookAt(global::directionalLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	volumetricLightShader = std::make_unique<VolumetricLightShader>();
	phongShader = std::make_unique<PhongShader>();
	depthShader = std::make_unique<DepthShader>();

	modelManager = std::make_unique<ModelManager>();

	// return 0 if everything is fine
	return 0;
}