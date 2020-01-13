#include "AutomaticCamera.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <array>

struct ControlPoint {
	glm::vec3 position;
	glm::vec3 rotation;
	float t;
};

struct TransitionPoint {
	float startTime;
	float endTime;
	bool fadeIn;
};

// VERY IMPORTANT
// the t value describes the time length of a segment
std::vector<std::vector<ControlPoint>> curves {
	{
		{ glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(-0.56f, 1.28f, 0.0f)							, 0.01 },
		{ glm::vec3(-31.91f, 2.32189f, 50.084f), glm::vec3(-36.88f, 5.12f, 0.0f)				, 0.01 },
		{ glm::vec3(-20.7824f, 5.11993f, -12.1709f), glm::vec3(-123.12f, 8.32f, 0.0f)			, 0.01 },
		{ glm::vec3(1.43385f, 8.71947f, -6.86863f), glm::vec3(-192.16f, 17.92f, 0.0f)			, 0.01 },
		{ glm::vec3(0.0446262f, 10.0761f, 4.52153f), glm::vec3(-39.68f, -20.8f, 0.0f)			, 0.01 },
		{ glm::vec3(14.1461f, 10.0225f, -0.922727f), glm::vec3(91.4401f, -6.72002f, 0.0f)		, 0.01 }
	},
	{
		{ glm::vec3(-17.2877f, -7.03546f, 19.0856f), glm::vec3(-41.7601f, 44.1599f, 0.0f)		, 0.01 },
		{ glm::vec3(-6.05232f, 12.2233f, 5.95136f), glm::vec3(-42.96f, 25.4399f, 0.0f)			, 0.01 },
		{ glm::vec3(-0.39438f, 16.0422f, 0.504072f), glm::vec3(-59.2001f, -18.6401f, 0.0f)		, 0.01 },
		{ glm::vec3(2.33357f, 14.966f, -0.934069f), glm::vec3(-96.5601f, -49.76f, 0.0f)			, 0.01 },
		{ glm::vec3(3.7011f, 13.8594f, -0.403036f), glm::vec3(-163.36f, -35.84f, 0.0f)			, 0.01 },
		{ glm::vec3(4.1915f, 12.1712f, 2.29714f), glm::vec3(-247.52f, -21.92f, 0.0f)			, 0.01 },
		{ glm::vec3(1.43645f, 11.3141f, 3.51381f), glm::vec3(-300.32f, -21.2f, 0.0f)			, 0.01 },
		{ glm::vec3(-2.79711f, 10.1155f, 0.668185f), glm::vec3(-396.16f, 10.48f, 0.0f)			, 0.01 }
	},
	{
		{ glm::vec3(-121.109f, -0.348613f, 44.2004f), glm::vec3(-57.84f, -12.88f, 0.0f)			, 0.01 },
		{ glm::vec3(-73.8757f, -0.348613f, 86.7607f), glm::vec3(-30.08f, -8.64001f, 0.0f)		, 0.01 },
		{ glm::vec3(-3.14925f, -0.348613f, 101.657f), glm::vec3(2.55998f, -9.92001f, 0.0f)		, 0.01 },
		{ glm::vec3(75.4952f, 7.90453f, 73.9174f), glm::vec3(47.36f, -8.88001f, 0.0f)			, 0.01 },
		{ glm::vec3(102.009f, 12.828f, 51.9475f), glm::vec3(81.12f, -23.92f, 0.0f)				, 0.01 },
		{ glm::vec3(110.497f, 14.5025f, 12.1423f), glm::vec3(84.88f, -7.36001f, 0.0f)			, 0.01 },
		{ glm::vec3(110.558f, 15.6f, -18.3584f), glm::vec3(119.12f, -19.04f, 0.0f)				, 0.01 },
		{ glm::vec3(102.801f, 24.2902f, -54.9019f), glm::vec3(119.76f, -17.52f, 0.0f)			, 0.01 },
		{ glm::vec3(33.3217f, 24.2902f, -110.797f), glm::vec3(164.72f, -18.64f, 0.0f)			, 0.01 },
		{ glm::vec3(-34.7239f, 24.2902f, -114.123f), glm::vec3(198.801f, -17.28f, 0.0f)			, 0.01 },
		{ glm::vec3(-67.8184f, 20.52f, -72.105f), glm::vec3(244.161f, -14.96f, 0.0f)			, 0.01 },
		{ glm::vec3(-58.0856f, 16.3885f, -19.657f), glm::vec3(284.641f, -13.12f, 0.0f)			, 0.01 },
		{ glm::vec3(-23.5649f, 8.35816f, 8.85487f), glm::vec3(269.521f, -25.36f, 0.0f)			, 0.01 },
		{ glm::vec3(-2.68269f, -0.930763f, 17.7269f), glm::vec3(239.841f, -21.44f, 0.0f)		, 0.01 },
		{ glm::vec3(21.1806f, -9.55727f, 8.22238f), glm::vec3(352.961f, -11.28f, 0.0f)			, 0.01 },
		{ glm::vec3(12.647f, -13.2829f, -17.6487f), glm::vec3(386.881f, 13.6f, 0.0f)			, 0.01 },
		{ glm::vec3(-1.74525f, -0.896129f, -18.6995f), glm::vec3(512.801f, 36.8f, 0.0f)			, 0.01 }
	}
};

// black fade in/out transitions (only in mode 2)
// start values, end values must be in order!
// start time, end time, fade in/out
std::vector<TransitionPoint> transitions = {
	{ 0.00, 0.02, true },
	{ 0.95, 1.0, false }
};

TransitionPoint currentTransitionPoint;
int currentTransitionPointIndex;
std::array<ControlPoint, 4> currentControlPoints;

std::array<int, 2> currentIndices;

glm::vec3 previousRotation;

AutomaticCamera::AutomaticCamera()
{
	currentTransitionPoint = transitions[0];
	currentTransitionPointIndex = 0;

	float currentT = 0.0f;
	float temp;
	for (auto & curve : curves)
	{
		for (auto & controlPoint : curve)
		{
			temp = controlPoint.t;
			controlPoint.t = currentT;
			currentT += temp;
		}
		currentT -= temp;
	}
	auto &lastCurve = curves[curves.size() - 1];
	lastCurve[lastCurve.size() - 1].t = 1.0;

	for (auto & curve : curves)
	{
		curve.insert(curve.begin(), curve.at(0));
		curve.push_back(curve.at(curve.size() - 1));
	}

	currentIndices = {0,0};

	currentControlPoints[0] = curves[0][0];
	currentControlPoints[1] = curves[0][1];
	currentControlPoints[2] = curves[0][2];
	currentControlPoints[3] = curves[0][3];

	this->position = currentControlPoints[1].position;

	glm::vec3 rotation = currentControlPoints[1].rotation;

	setYaw(rotation.x);
	setPitch(rotation.y);
	setRoll(rotation.z);
}

AutomaticCamera::~AutomaticCamera()
{
}

void AutomaticCamera::update()
{
	if (currentTransitionPointIndex < transitions.size() && global::t > currentTransitionPoint.endTime)
	{
		currentTransitionPointIndex++;
		if (currentTransitionPointIndex < transitions.size())
		{
			currentTransitionPoint = transitions[currentTransitionPointIndex];
		}
	}

	if (global::t >= currentTransitionPoint.startTime && global::t <= currentTransitionPoint.endTime)
	{
		global::currentTransitionGrayness = (global::t - currentTransitionPoint.startTime) / (currentTransitionPoint.endTime - currentTransitionPoint.startTime);
		if (currentTransitionPoint.fadeIn) global::currentTransitionGrayness = 1.0 - global::currentTransitionGrayness;
	}
	else
	{
		global::currentTransitionGrayness = 0.0;
	}

	if (global::t > currentControlPoints[2].t)
	{
		if (currentIndices[1] + 4 > curves[currentIndices[0]].size() - 1)
		{
			currentIndices[0] = currentIndices[0]++;
			currentIndices[1] = 0;
		}
		else
		{
			currentIndices[1] = currentIndices[1]++;
		}

		currentControlPoints[0] = curves[currentIndices[0]][currentIndices[1]];
		currentControlPoints[1] = curves[currentIndices[0]][currentIndices[1] + 1];
		currentControlPoints[2] = curves[currentIndices[0]][currentIndices[1] + 2];
		currentControlPoints[3] = curves[currentIndices[0]][currentIndices[1] + 3];
	}

	double localT = (global::t - currentControlPoints[1].t) * (1.0 / (currentControlPoints[2].t - currentControlPoints[1].t));

	this->position = glm::catmullRom(currentControlPoints[0].position, currentControlPoints[1].position, currentControlPoints[2].position, currentControlPoints[3].position, localT);
	glm::vec3 newRotation = glm::catmullRom(currentControlPoints[0].rotation, currentControlPoints[1].rotation, currentControlPoints[2].rotation, currentControlPoints[3].rotation, localT);

	setYaw(newRotation.x);
	setPitch(newRotation.y);
	setRoll(newRotation.z);
}

void AutomaticCamera::processInput()
{
}

void AutomaticCamera::processMouseMovement(double xoffset, double yoffset)
{
}