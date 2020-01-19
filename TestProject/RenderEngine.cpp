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

#include "VoidShader.h"
#include "ForcefieldShader.h"
#include "CombineShader.h"
#include "BloomShader.h"
#include "BlurShader.h"
#include "VolumentricLightShader.h"
#include "PhongShader.h"
#include "DepthShader.h"

#include "Skybox.h"
#include "ModelManager.h"
#include "FrameBuffer.h"
#include "ParticleSystem.h"

std::unique_ptr<VoidShader> voidShader;
std::unique_ptr<ForcefieldShader> forcefieldShader;
std::unique_ptr<CombineShader> combineShader;
std::unique_ptr<BloomShader> bloomShader;
std::unique_ptr<VolumetricLightShader> volumetricLightShader;
std::unique_ptr<PhongShader> phongShader;
std::unique_ptr<DepthShader> depthShader;

std::unique_ptr<ModelManager> modelManager;

std::unique_ptr<FrameBuffer> phongResult, bloomResult, volumetricLightingResult;

std::vector<std::unique_ptr<ParticleSystem>> particleSystems;

RenderEngine::RenderEngine()
{
}


RenderEngine::~RenderEngine()
{
}

void addConfetti() {
	particleSystems.push_back(std::make_unique<ParticleSystem>(glm::vec3(25.0 + 6.0, -25.0, 25.0 + 6.0), 1024, glm::vec3(3.0, 3.0, 3.0), 1, glm::vec3(5.0, 45.0, 5.0)));
	particleSystems.push_back(std::make_unique<ParticleSystem>(glm::vec3(-25.0 - 6.0, -25.0, 25.0 + 6.0), 1024, glm::vec3(3.0, 3.0, 3.0), 1, glm::vec3(-5.0, 45.0, 5.0)));
	particleSystems.push_back(std::make_unique<ParticleSystem>(glm::vec3(25.0 + 6.0, -25.0, -25.0 - 6.0), 1024, glm::vec3(3.0, 3.0, 3.0), 1, glm::vec3(5.0, 45.0, -5.0)));
	particleSystems.push_back(std::make_unique<ParticleSystem>(glm::vec3(-25.0 - 6.0, -25.0, -25.0 - 6.0), 1024, glm::vec3(3.0, 3.0, 3.0), 1, glm::vec3(-5.0, 45.0, -5.0)));
}

void RenderEngine::run(std::promise<int> && error)
{
	error.set_value(0);

	if (init())
	{
		error.set_value(-1);
		return;
	}

	while (!global::windowShouldClose && global::t < 1.0)
	{
		update();
		render();
	}

	glfwTerminate();
}

float previousT = 0.0f;

void RenderEngine::update()
{
	global::windowShouldClose = glfwWindowShouldClose(global::window);
	
	currentFrame = glfwGetTime();
	global::deltaTimeRenderEngine = currentFrame - lastFrame;
	lastFrame = currentFrame;
	global::currentFrameLogicEngine = currentFrame;

	global::t = 1.0f - ((songLength - (currentFrame - startTime)) / songLength);
	if (global::t > 1.0f) global::t = 1.0f;

	processInput();
	global::camera->update();

	// time specific events
	if (global::t > 0.297 && previousT < 0.297)
	{
		global::spotLightsOn = true;
		addConfetti();
	}
	if (global::t > 0.44 && previousT < 0.44)
	{
		global::spotLightsOn = false;
	}
	if (global::t > 0.375 && previousT < 0.375) addConfetti();
	if (global::t > 0.745 && previousT < 0.745)
	{
		global::spotLightsOn = true;
		addConfetti();
	}
	if (global::t > 0.745 && global::t < 0.97)
	{
		if (int(global::t * 40000) % 10 < 5) global::spotLightsOn = false;
		else global::spotLightsOn = true;
	}
	if (global::t > 0.97 && previousT < 0.97) global::spotLightsOn = false;
	if (global::t > 0.825 && previousT < 0.825) addConfetti();

	// update light position -------------------------------------------------------------------------------------------------------------------------------

	global::directionalLight->direction = glm::rotateX(glm::vec3(0.0f, 0.0f, 0.5f), glm::radians(730.0f * (float)global::t));
	global::directionalLight->position = global::directionalLight->direction * (-1000.0f);
	global::directionalLight->view = glm::lookAt(global::directionalLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < 4; i++)
	{
		global::spotLights[i]->direction = global::spotLights[i]->startDirection * glm::vec3(cos(glm::radians((float)global::t * 360 * 50)) * 0.5 + 0.7, 1.0, cos(glm::radians((float)global::t * 360 * 50)) * 0.5 + 0.7);
	}

	// update breakout -------------------------------------------------------------------------------------------------------------------------------------
	modelManager->ball->model = glm::translate(glm::mat4(1.0f), glm::vec3(breakout::ballPosition));
	modelManager->pad->model = glm::translate(glm::mat4(1.0f), glm::vec3(breakout::padPosition));

	modelManager->bricks->update();

	// update particle systems -----------------------------------------------------------------------------------------------------------------------------
	global::particleSystemsToAddMutex.lock();
	for (int i = 0; i < global::particleSystemsToAdd.size(); i++)
	{
		particleSystems.push_back(std::make_unique<ParticleSystem>(global::particleSystemsToAdd[i].position, global::particleSystemsToAdd[i].particleCount, global::particleSystemsToAdd[i].color));
	}
	global::particleSystemsToAdd.clear();
	global::particleSystemsToAddMutex.unlock();

	for (int i = 0; i < particleSystems.size(); i++)
	{
		if (particleSystems[i]->current_particle_count == 0)
		{
			particleSystems.erase(particleSystems.begin() + i, particleSystems.begin() + i + 1);
			i--;
		}
		else
		{
			particleSystems[i]->update(global::deltaTimeRenderEngine);
		}
	}

	previousT = global::t;
}

void RenderEngine::render()
{
	// opengl settings, view/projection calculation --------------------------------------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	phongResult->bind();
	bloomResult->bind();
	volumetricLightingResult->bind();

	glm::mat4 view = global::camera->getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)setting::SCREEN_WIDTH / (float)setting::SCREEN_HEIGHT, 0.1f, 2000.0f);

	// depthmap --------------------------------------------------------------------------------------------------------------------------------------------
	depthShader->use(global::directionalLight->view, global::directionalLight->projection);
	depthShader->draw(*modelManager->map);

	if (global::t > 0.11)
	{
		breakout::bricksPositionMutex.lock();
		phongShader->drawInstanced(*modelManager->brick, breakout::bricksPosition.size());
		breakout::bricksPositionMutex.unlock();
		if (global::t > 0.135)
		{
			depthShader->draw(*modelManager->ball);
			depthShader->draw(*modelManager->pad);
		}
	}

	depthShader->finish();

	// illumination, skybox and forcefield -----------------------------------------------------------------------------------------------------------------------------
	phongShader->use(view, projection, depthShader->depthmap);
	phongShader->draw(*modelManager->map);

	if (global::t > 0.11)
	{
		if (global::t > 0.135)
		{
			phongShader->draw(*modelManager->ball, true);
			phongShader->draw(*modelManager->pad);
		}
		breakout::bricksPositionMutex.lock();
		phongShader->drawInstanced(*modelManager->brick, breakout::bricksPosition.size());
		breakout::bricksPositionMutex.unlock();
	}

	drawSkybox(view, projection);

	voidShader->draw(view, projection);
	forcefieldShader->drawForcefield(view, projection);

	// particle systems ------------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < particleSystems.size(); i++)
	{
		particleSystems[i]->draw(view, projection);
	}

	// copy msaa rendered scene into normal framebuffer ----------------------------------------------------------------------------------------------------
	phongResult->bind();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, phongShader->framebuffer->FBO);
	glBlitFramebuffer(0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, 0, 0, setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// volumetric lighting ---------------------------------------------------------------------------------------------------------------------------------
	volumetricLightShader->perform(view, projection, depthShader->depthmap, phongResult->FBOdepthmap);
	combineShader->combine(volumetricLightShader->blurShader->blurredTexture, phongResult->FBOtexture, volumetricLightingResult->FBO, 1);

	// bloom -----------------------------------------------------------------------------------------------------------------------------------------------
	bloomShader->bloom(phongResult->FBOtexture);
	combineShader->combine(volumetricLightingResult->FBOtexture, bloomShader->blurShader->blurredTexture, 0, 2);

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

	// opengl settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	initSkybox();
	
	// light initialization
	global::directionalLight = std::make_unique<DirectionalLight>();
	global::directionalLight->direction = glm::vec3(0.0f, 0.0f, 0.5f);
	global::directionalLight->position = global::directionalLight->direction * (-1000.0f);
	global::directionalLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	global::directionalLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	global::directionalLight->specular = glm::vec3(0.8f, 0.8f, 0.8f);

	global::directionalLight->projection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 10000.0f);
	global::directionalLight->view = glm::lookAt(global::directionalLight->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	global::spotLights[0] = std::make_unique<SpotLight>(glm::vec3(25, 25, 0), glm::vec3(1, -1, 0), 5.0f, 30.0f, 0.2, 0.00001, 0.00001, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	global::spotLights[1] = std::make_unique<SpotLight>(glm::vec3(-25, 25, 0), glm::vec3(-1, -1, 0), 5.0f, 30.0f, 0.2, 0.00001, 0.00001, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	global::spotLights[2] = std::make_unique<SpotLight>(glm::vec3(0, 25, 25), glm::vec3(0, -1, 1), 5.0f, 30.0f, 0.2, 0.00001, 0.00001, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	global::spotLights[3] = std::make_unique<SpotLight>(glm::vec3(0, 25, -25), glm::vec3(0, -1, -1), 5.0f, 30.0f, 0.2, 0.00001, 0.00001, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// load shader, model and create framebuffers
	voidShader = std::make_unique<VoidShader>();
	forcefieldShader = std::make_unique<ForcefieldShader>();
	combineShader = std::make_unique<CombineShader>();
	bloomShader = std::make_unique<BloomShader>();
	volumetricLightShader = std::make_unique<VolumetricLightShader>();
	phongShader = std::make_unique<PhongShader>();
	depthShader = std::make_unique<DepthShader>();

	modelManager = std::make_unique<ModelManager>();

	phongResult = std::make_unique<FrameBuffer>(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_RGBA16F, GL_FLOAT);
	bloomResult = std::make_unique<FrameBuffer>(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_RGBA16F, GL_FLOAT);
	volumetricLightingResult = std::make_unique<FrameBuffer>(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, GL_RGBA16F, GL_FLOAT);

	// enable or disable music
	global::SoundEngine = createIrrKlangDevice();

	global::candylandSong = global::SoundEngine->play2D("../assets/Candyland-Tobu.mp3", false, false, ESM_NO_STREAMING);

	

	if (!setting::MUSIC)
		global::SoundEngine->setSoundVolume(0.0f);
	else
		global::SoundEngine->setSoundVolume(1.0f);

	const float speedup = 1.0f;

	songLength = global::candylandSong->getPlayLength() / (1000.0f * speedup);

	//max value 0.53f
	const float skipT = 0.0f;
	global::t = skipT;

	global::candylandSong->setPlaybackSpeed(speedup);
	global::candylandSong->setPlayPosition((skipT * songLength) * (1000.0f / speedup));

	startTime = glfwGetTime() - (skipT * songLength);
	lastFrame = startTime;

	// return 0 if everything is fine
	return 0;
}