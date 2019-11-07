#pragma once
#include <future>

class RenderEngine
{
public:
	RenderEngine();
	~RenderEngine();

	void run(std::promise<int> && error);
	void update();
	void render();
	int init();
private:
	float currentFrame, lastFrame;
};

