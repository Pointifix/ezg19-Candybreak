#pragma once
#include <iostream>
#include <atomic>
#include <array>
#include <queue>
#include <mutex>

#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>
#include "Camera.h"
#include "DirectionalLight.h"

using namespace irrklang;

template<typename T, typename Container = std::deque<T>>
class iterable_queue : public std::queue<T, Container>
{
public:
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
	const_iterator begin() const { return this->c.begin(); }
	const_iterator end() const { return this->c.end(); }
};

namespace global
{
	extern GLFWwindow* window;
	extern ISoundEngine *SoundEngine;
	extern ISound *candylandSong;

	extern std::unique_ptr<Camera> camera;

	extern std::unique_ptr<DirectionalLight> directionalLight;

	extern GLuint screenQuadVAO;

	extern std::atomic<bool> windowShouldClose;

	// time related values
	extern std::atomic<double> deltaTimeRenderEngine; // dont access from another thread than render engine thread
	extern std::atomic<double> deltaTimeLogicEngine; // dont access from another thread than logic engine thread
	extern std::atomic<double> currentFrameLogicEngine; // dont access from another thread than logic engine thread
	extern std::atomic<double> t;

	// particle system
	extern std::mutex particleSystemsToAddMutex;
	struct particleSystem {
		glm::vec3 position;
		int particleCount;
		glm::vec3 color;
	};
	extern std::vector<particleSystem> particleSystemsToAdd;
}

namespace breakout
{
	const double BOX_SIZE = 50.0;
	const int BREAKOUT_SIZE = 10;

	extern std::array<std::array<std::array<std::atomic<bool>, BREAKOUT_SIZE>, BREAKOUT_SIZE>, BREAKOUT_SIZE> bricksBool;

	extern std::atomic<bool> updateBricks;
	extern std::mutex bricksToRemoveMutex;
	extern std::vector<glm::vec3> bricksToRemove;

	extern std::mutex bricksPositionMutex;
	extern std::vector<glm::vec3> bricksPosition;

	extern std::atomic<glm::vec3> ballPosition, padPosition;

	extern std::mutex hitQueuesMutex;
	extern iterable_queue<glm::vec3> hitQueues[5];
}

namespace setting
{
	extern unsigned int SCREEN_WIDTH;
	extern unsigned int SCREEN_HEIGHT;

	extern bool FULLSCREEN;
	extern bool CONSOLE;
	extern bool MUSIC;

	extern std::string CAMERA_MODE;

	extern std::string TITLE;
}