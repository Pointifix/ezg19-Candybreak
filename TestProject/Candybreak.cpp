#pragma once
#include <thread>
#include <future>
#include <SettingsReader.h>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <irrKlang/irrKlang.h>

#include "Globals.h"
#include "LogicEngine.h"
#include "RenderEngine.h"
#include "InputHandler.h"

int readSettings();

int main()
{
	if (readSettings()) return -1;

	LogicEngine logicEngine;
	RenderEngine renderEngine;

	std::thread logicEngineThread(&LogicEngine::run, logicEngine);

	std::promise<int> error;
	auto future = error.get_future();

	std::thread renderEngineThread(&RenderEngine::run, renderEngine, std::move(error));

	logicEngineThread.join();
	renderEngineThread.join();

	return future.get();
}

int readSettings()
{
	INIReader reader("../assets/settings.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Failed to load 'settings.ini'" << std::endl;
		return -1;
	}

	setting::SCREEN_WIDTH = reader.GetInteger("window", "width", 1024);
	setting::SCREEN_HEIGHT = reader.GetInteger("window", "height", 768);
	setting::FULLSCREEN = reader.GetBoolean("window", "fullscreen", false);
	setting::TITLE = reader.Get("window", "title", "Candybreak");
	setting::CONSOLE = reader.GetBoolean("window", "console", true);
	setting::MUSIC = reader.GetBoolean("music", "theme", true);
	setting::CAMERA_MODE = reader.Get("camera", "mode", "free");

	return 0;
}