#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Init.h"

#include <iostream>

#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SettingsReader.h>
#include <irrKlang/irrKlang.h>
#include "InputHandler.h"

using namespace irrklang;

GLFWwindow* window;
ISoundEngine *SoundEngine;

glm::vec2 screenSize;

namespace setting
{
	unsigned int SCREEN_WIDTH;
	unsigned int SCREEN_HEIGHT;
	bool FULLSCREEN;
	std::string TITLE;
	unsigned int REFRESH_RATE;
	float BRIGHTNESS;
	bool FREE_CAMERA;
	bool CONSOLE;
	bool THEME;
}

int init()
{
	// read settings from settings.ini
	// -------------------------------
	INIReader reader("../assets/settings.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Failed to load 'settings.ini'" << std::endl;
		return -1;
	}

	setting::SCREEN_WIDTH = reader.GetInteger("window", "width", 1024);
	setting::SCREEN_HEIGHT = reader.GetInteger("window", "height", 768);
	setting::FULLSCREEN = reader.GetBoolean("window", "fullscreen", false);
	setting::TITLE = reader.Get("window", "title", "Candybreak");
	setting::REFRESH_RATE = reader.GetReal("window", "refresh rate", 60);
	setting::BRIGHTNESS = float(reader.GetReal("window", "brightness", 1.0));
	setting::FREE_CAMERA = reader.GetBoolean("camera", "free", true);
	setting::CONSOLE = reader.GetBoolean("window", "console", true);
	setting::THEME = reader.GetBoolean("music", "theme", true);

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	if (setting::FULLSCREEN)
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		glfwSetGamma(primary, setting::BRIGHTNESS);
		window = glfwCreateWindow(1980, 1080, setting::TITLE.c_str(), primary, nullptr);
		screenSize = glm::vec2(1980, 1080);
	}
	else
	{
		window = glfwCreateWindow(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT, setting::TITLE.c_str(), nullptr, nullptr);
		screenSize = glm::vec2(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT);
	}

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// enable or disable console (presentation purpose)
	// ------------------------------------------------
	if (setting::CONSOLE)
	{
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	}
	else
	{
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	}

	// enable or disable theme song (presentation purpose)
	// ---------------------------------------------------
	SoundEngine = createIrrKlangDevice();

	if (!setting::THEME)
		SoundEngine->setSoundVolume(0.0f);

	return 0;
}