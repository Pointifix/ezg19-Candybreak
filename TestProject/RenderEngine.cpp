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
#include "AutomaticCamera.h"
#include "InputHandler.h"

#include "CombineShader.h"
#include "BloomShader.h"
#include "BlurShader.h"
#include "VolumentricLightShader.h"
#include "PhongShader.h"
#include "DepthShader.h"

#include "Skybox.h"
#include "ModelManager.h"
#include "FrameBuffer.h"

std::unique_ptr<CombineShader> combineShader;
std::unique_ptr<BloomShader> bloomShader;
std::unique_ptr<BlurShader> blurShader;
std::unique_ptr<VolumetricLightShader> volumetricLightShader;
std::unique_ptr<PhongShader> phongShader;
std::unique_ptr<DepthShader> depthShader;

std::unique_ptr<ModelManager> modelManager;

std::unique_ptr<FrameBuffer> offScreenFrameBuffer;

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

	while (!global::windowShouldClose)
	{
		update();
		render();
	}

	glfwTerminate();
}

void RenderEngine::update()
{
	global::windowShouldClose = glfwWindowShouldClose(global::window);

	currentFrame = glfwGetTime();
	global::deltaTimeRenderEngine = currentFrame - lastFrame;
	lastFrame = currentFrame;

	global::t = 1.0f - ((songLength - (currentFrame - startTime)) / songLength);
	if (global::t > 1.0f) global::t = 1.0f;

	processInput();

	modelManager->ball->model = glm::translate(glm::mat4(1.0f), glm::vec3(breakout::ballPosition));
	modelManager->pad->model = glm::translate(glm::mat4(1.0f), glm::vec3(breakout::padPosition));

	modelManager->bricks->update();

	global::camera->update();
}

void RenderEngine::render()
{
	// opengl settings, view/projection calculation --------------------------------------------------------------------------------------------------------
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, offScreenFrameBuffer->FBO);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 view = global::camera->getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)setting::SCREEN_WIDTH / (float)setting::SCREEN_HEIGHT, 0.1f, 500.0f);

	// depthmap --------------------------------------------------------------------------------------------------------------------------------------------

	depthShader->use(global::directionalLight->view, global::directionalLight->projection);
	depthShader->draw(*modelManager->map);
	depthShader->draw(*modelManager->ball);
	depthShader->draw(*modelManager->pad);
	depthShader->finish();

	// illumination and skybox -----------------------------------------------------------------------------------------------------------------------------
	phongShader->use(view, projection, depthShader->depthmap);
	phongShader->draw(*modelManager->map);
	phongShader->draw(*modelManager->ball);
	phongShader->draw(*modelManager->pad);
	phongShader->draw(*modelManager->light);

	breakout::bricksPositionMutex.lock();
	phongShader->drawInstanced(*modelManager->brick, breakout::bricksPosition.size());
	breakout::bricksPositionMutex.unlock();

	//drawSkybox(view, projection);

	// bloom -----------------------------------------------------------------------------------------------------------------------------------------------

	bloomShader->bloom(phongShader->framebuffer->FBOtexture);
	blurShader->blur(bloomShader->framebuffer->FBOtexture, 2);

	combineShader->combine(phongShader->framebuffer->FBOtexture, blurShader->blurredTexture, 0);

	// volumetric lighting ---------------------------------------------------------------------------------------------------------------------------------

	//volumetricLightShader->perform(view, projection, depthShader->depthmap, phongShader->framebuffer->FBOdepthmap);
	//blurShader->blur(volumetricLightShader->framebuffer->FBOtexture, 2);

	// combine ---------------------------------------------------------------------------------------------------------------------------------------------

	//combineShader->combine(offScreenFrameBuffer->FBOtexture, blurShader->blurredTexture, 0);

	// swap buffers, poll events --------------------------------------------------------------------------------------------------------------------------
	glfwSwapBuffers(global::window);
	glfwPollEvents();
}

int RenderEngine::init()
{
	if (setting::CAMERA_MODE == "auto") global::camera = std::make_unique<AutomaticCamera>();
	if (global::camera == nullptr) global::camera = std::make_unique<FreeCamera>(glm::vec3(0.0f, 0.0f, 100.0f));

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// glfw window creation
	if (setting::FULLSCREEN)
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		global::window = glfwCreateWindow(1980, 1080, setting::TITLE.c_str(), primary, nullptr);

		setting::SCREEN_WIDTH = 1980;
		setting::SCREEN_HEIGHT = 1080;
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

	songLength = global::candylandSong->getPlayLength() / 1000.0f;

	startTime = glfwGetTime();
	lastFrame = startTime;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	initSkybox();
	
	global::directionalLight = std::make_unique<DirectionalLight>();
	global::directionalLight->direction = glm::vec3(0.5f, -0.5f, -0.5f);
	global::directionalLight->position = global::directionalLight->direction * (-500.0f);
	global::directionalLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	global::directionalLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	global::directionalLight->specular = glm::vec3(0.8f, 0.8f, 0.8f);

	global::directionalLight->projection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 1000.0f);
	global::directionalLight->view = glm::lookAt(global::directionalLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	combineShader = std::make_unique<CombineShader>();
	bloomShader = std::make_unique<BloomShader>();
	blurShader = std::make_unique<BlurShader>(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT);
	volumetricLightShader = std::make_unique<VolumetricLightShader>();
	phongShader = std::make_unique<PhongShader>();
	depthShader = std::make_unique<DepthShader>();

	modelManager = std::make_unique<ModelManager>();

	offScreenFrameBuffer = std::make_unique<FrameBuffer>(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_RGBA16F, GL_FLOAT);

	// return 0 if everything is fine
	return 0;
}