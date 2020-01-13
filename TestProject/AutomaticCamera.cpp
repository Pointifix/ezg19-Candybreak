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
		{ glm::vec3(6.08515f, -8.22027f, 528.232f), glm::vec3(-0.400001f, -2.80001f, 0.0f), 0.01 },
		{ glm::vec3(8.10513f, -10.6716f, 345.332f), glm::vec3(0.399999f, 1.67999f, 0.0f), 0.01 },
		{ glm::vec3(-12.9545f, -7.19209f, 190.49f), glm::vec3(-4.32f, -0.480012f, 0.0f), 0.01 },
		{ glm::vec3(5.79986f, -8.53529f, 99.6339f), glm::vec3(-29.84f, -1.92001f, 3.49124f), 0.01 },
		{ glm::vec3(44.0426f, -12.0314f, 48.8162f), glm::vec3(-41.92f, -4.08001f, -13.5041f), 0.01 },
		{ glm::vec3(63.9712f, -17.2962f, -23.0276f), glm::vec3(5.83998f, -3.76001f, -18.1175f), 0.015 },
		{ glm::vec3(48.4364f, -20.236f, -76.4254f), glm::vec3(98.48f, -1.20001f, -14.2328f), 0.015 },
		{ glm::vec3(6.19351f, -19.0798f, -95.4774f), glm::vec3(171.2f, -0.320021f, -5.9279f), 0.015 },
		{ glm::vec3(-58.5279f, -17.4675f, -92.4687f), glm::vec3(207.6f, 11.76f, -2.91814f), 0.015 },
	},
	{
		{ glm::vec3(-40.2953f, 9.17974f, 48.6856f), glm::vec3(-40.8001f, 8.71999f, 0.0f), 0.02 },
		{ glm::vec3(-41.3118f, 3.5167f, 97.002f), glm::vec3(-22.8001f, 3.67999f, 0.0f), 0.02 },
		{ glm::vec3(-26.0417f, 1.08776f, 135.801f), glm::vec3(-12.7201f, 1.51999f, 0.0f), 0.02 },
		{ glm::vec3(-22.2948f, -0.267128f, 198.856f), glm::vec3(-7.68007f, -0.720006f, 0.0f), 0.02 }
	},
	{
		{ glm::vec3(-47.7308f, -5.14316f, 15.7767f), glm::vec3(-86.72f, -17.36f, 0.0f), 0.01 },
		{ glm::vec3(-25.3525f, -5.14316f, 54.0715f), glm::vec3(-36.56f, -17.52f, 0.0f), 0.01 },
		{ glm::vec3(28.9631f, -0.786949f, 81.6015f), glm::vec3(10.96f, -17.04f, 0.0f), 0.01 }
	},
	{
		{ glm::vec3(-9.61441f, -3.55723f, 142.464f), glm::vec3(-8.48f, -37.4399f, 0.0f), 0.01 },
		{ glm::vec3(-3.72208f, -19.2102f, 109.785f), glm::vec3(-10.24f, -5.35993f, 0.0f), 0.01 },
		{ glm::vec3(2.63327f, -18.5013f, 70.8266f), glm::vec3(-10.8f, 7.04007f, 0.0f), 0.01 },
		{ glm::vec3(10.6918f, -10.3934f, 28.5822f), glm::vec3(-10.8f, 30.0001f, 0.0f), 0.01 },
		{ glm::vec3(13.218f, 1.34838f, 15.0181f), glm::vec3(-8.23999f, 70.7201f, 0.0f), 0.01 },
		{ glm::vec3(13.6715f, 21.2977f, 11.4721f), glm::vec3(-5.52f, 81.2801f, 0.0f), 0.01 },
		{ glm::vec3(14.1666f, 40.6691f, 3.07039f), glm::vec3(-0.0799975f, 4.48009f, 0.0f), 0.01 },
		{ glm::vec3(14.5759f, 22.4701f, -13.7353f), glm::vec3(5.68f, -87.8399f, 0.0f), 0.01 },
		{ glm::vec3(14.5261f, 0.590445f, -14.1951f), glm::vec3(46.24f, -64.5599f, 0.0f), 0.01 },
		{ glm::vec3(-5.10614f, -19.33f, -23.7767f), glm::vec3(108.88f, 10.2401f, -4.15411f), 0.01 },
		{ glm::vec3(-21.5454f, -12.2036f, -3.76379f), glm::vec3(197.04f, 3.2801f, -8.36624f), 0.01 },
		{ glm::vec3(-9.33559f, -9.71817f, 18.7713f), glm::vec3(244.16f, 4.6401f, -10.7893f), 0.01 },
		{ glm::vec3(8.17783f, -0.43684f, 66.2926f), glm::vec3(342.0f, -5.1199f, -7.17213f), 0.01 }
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
	{ 0.277, 0.297, false },
	{ 0.725, 0.745, false },
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
		ControlPoint add = curve.at(0);
		curve.insert(curve.begin(), { glm::vec3(add.position), glm::vec3(0.0), add.t });
		add = curve.at(curve.size() - 1);
		curve.push_back({ glm::vec3(add.position), glm::vec3(0.0), add.t });
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