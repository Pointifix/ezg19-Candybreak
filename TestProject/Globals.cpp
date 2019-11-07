#pragma once
#include "Globals.h"

namespace global
{
	GLFWwindow* window;
	ISoundEngine *SoundEngine;
	ISound *candylandSong;

	Camera* camera;

	std::unique_ptr<DirectionalLight> directionalLight;

	float deltaTime;
	float t;
}

namespace setting
{
	unsigned int SCREEN_WIDTH;
	unsigned int SCREEN_HEIGHT;

	bool FULLSCREEN;
	bool CONSOLE;
	bool MUSIC;

	std::string TITLE;
}