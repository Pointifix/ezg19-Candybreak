#pragma once
class LogicEngine
{
public:
	LogicEngine();
	~LogicEngine();
	void run();
private:
	double currentFrame, lastFrame;
};