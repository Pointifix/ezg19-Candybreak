#pragma once
#include "LogicEngine.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Breakout.h"

const double LOGIC_UPDATE_RATE = 1.0 / 512.0;

std::unique_ptr<Breakout> breakoutObj;

LogicEngine::LogicEngine()
{
	breakoutObj = std::make_unique<Breakout>();
}

LogicEngine::~LogicEngine()
{
}

void LogicEngine::run()
{
	lastFrame = glfwGetTime();

	while (!global::windowShouldClose && global::t < 1.0)
	{
		currentFrame = glfwGetTime();
		global::deltaTimeLogicEngine = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// time measurement start
		if (global::t < 1.0 && global::t > 0.13)
		{
			breakoutObj->update();
		}

		// time measurement end + thread sleep

		currentFrame = glfwGetTime();
		if (currentFrame - lastFrame < LOGIC_UPDATE_RATE)
		{
			long sleepTime = (long)((LOGIC_UPDATE_RATE - (currentFrame - lastFrame)) * 1'000'000'000);

			std::this_thread::sleep_for(std::chrono::nanoseconds(sleepTime));
		}
	}
}
