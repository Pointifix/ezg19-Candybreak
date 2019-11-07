#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>
#include "Camera.h"
#include "DirectionalLight.h"

using namespace irrklang;

namespace global
{
	extern GLFWwindow* window;
	extern ISoundEngine *SoundEngine;
	extern ISound *candylandSong;

	extern Camera* camera;

	extern std::unique_ptr<DirectionalLight> directionalLight;

	extern float deltaTime;
	extern float t;
}

namespace setting
{
	extern unsigned int SCREEN_WIDTH;
	extern unsigned int SCREEN_HEIGHT;

	extern bool FULLSCREEN;
	extern bool CONSOLE;
	extern bool MUSIC;

	extern std::string TITLE;
}