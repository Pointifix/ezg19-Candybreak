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

// VERY IMPORTANT
// t value inside one curve must be strictly increasing
// last t value of each curve must equal the first t value of the next curve
// first curve must start at t = 0.0 and last curve must end at t = 1.0
std::vector<std::vector<ControlPoint>> curves {
	{
		{ glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(-0.56f, 1.28f, 0.0f)							, 0.0 },
		{ glm::vec3(-31.91f, 2.32189f, 50.084f), glm::vec3(-36.88f, 5.12f, 0.0f)				, 0.01 },
		{ glm::vec3(-20.7824f, 5.11993f, -12.1709f), glm::vec3(-123.12f, 8.32f, 0.0f)			, 0.02 },
		{ glm::vec3(1.43385f, 8.71947f, -6.86863f), glm::vec3(-192.16f, 17.92f, 0.0f)			, 0.03 },
		{ glm::vec3(0.0446262f, 10.0761f, 4.52153f), glm::vec3(-39.68f, -20.8f, 0.0f)			, 0.04 },
		{ glm::vec3(14.1461f, 10.0225f, -0.922727f), glm::vec3(91.4401f, -6.72002f, 0.0f)		, 0.05 }
	},
	{
		{ glm::vec3(-17.2877f, -7.03546f, 19.0856f), glm::vec3(-41.7601f, 44.1599f, 0.0f)		, 0.05 },
		{ glm::vec3(-6.05232f, 12.2233f, 5.95136f), glm::vec3(-42.96f, 25.4399f, 0.0f)			, 0.055 },
		{ glm::vec3(-0.39438f, 16.0422f, 0.504072f), glm::vec3(-59.2001f, -18.6401f, 0.0f)		, 0.06 },
		{ glm::vec3(2.33357f, 14.966f, -0.934069f), glm::vec3(-96.5601f, -49.76f, 0.0f)			, 0.065 },
		{ glm::vec3(3.7011f, 13.8594f, -0.403036f), glm::vec3(-163.36f, -35.84f, 0.0f)			, 0.07 },
		{ glm::vec3(4.1915f, 12.1712f, 2.29714f), glm::vec3(-247.52f, -21.92f, 0.0f)			, 0.08 },
		{ glm::vec3(1.43645f, 11.3141f, 3.51381f), glm::vec3(-300.32f, -21.2f, 0.0f)			, 0.09 },
		{ glm::vec3(-2.79711f, 10.1155f, 0.668185f), glm::vec3(-396.16f, 10.48f, 0.0f)			, 0.1 }
	},
	{
		{ glm::vec3(-77.0656f, 60.8783f, -48.8168f), glm::vec3(-478.48f, -33.6801f, 0.0f)		, 0.1 },
		{ glm::vec3(3.24297f, 35.1715f, -51.4954f), glm::vec3(-533.681f, -35.0401f, 0.0f)		, 0.13 },
		{ glm::vec3(37.5796f, 20.0717f, -5.57886f), glm::vec3(-621.361f, -23.3602f, 0.0f)		, 0.15 },
		{ glm::vec3(-0.720793f, 15.9863f, 26.979f), glm::vec3(-730.8f, -21.6802f, 0.0f)			, 0.17 }
	},
	{
		{ glm::vec3(-0.630547f, 14.1403f, 33.9937f), glm::vec3(-2.24f, -10.56f, 0.0f)			, 0.17 },
		{ glm::vec3(-0.630547f, 14.1403f, 33.9937f), glm::vec3(-2.24f, -10.56f, 52.0596f)		, 0.18 },
		{ glm::vec3(-6.72745f, 15.6847f, 16.3018f), glm::vec3(-40.24f, -28.64f, 80.9282f)		, 0.19 },
		{ glm::vec3(-13.9615f, 20.7674f, -0.095925f), glm::vec3(-118.08f, -34.7999f, 164.508f)	, 0.2 },
		{ glm::vec3(-13.9615f, 20.7674f, -0.095925f), glm::vec3(-123.36f, -31.9999f, 254.946f)	, 0.21 },
		{ glm::vec3(-13.9615f, 20.7674f, -0.095925f), glm::vec3(-119.6f, -41.52f, 353.515f)		, 0.22 },
		{ glm::vec3(-192.018f, 152.655f, -10.1558f), glm::vec3(-117.76f, -44.72f, 353.515f)		, 0.23 }
	},
	{
		{ glm::vec3(-121.109f, -0.348613f, 44.2004f), glm::vec3(-57.84f, -12.88f, 0.0f)			, 0.23 },
		{ glm::vec3(-73.8757f, -0.348613f, 86.7607f), glm::vec3(-30.08f, -8.64001f, 0.0f)		, 0.24 },
		{ glm::vec3(-3.14925f, -0.348613f, 101.657f), glm::vec3(2.55998f, -9.92001f, 0.0f)		, 0.25 },
		{ glm::vec3(75.4952f, 7.90453f, 73.9174f), glm::vec3(47.36f, -8.88001f, 0.0f)			, 0.26 },
		{ glm::vec3(102.009f, 12.828f, 51.9475f), glm::vec3(81.12f, -23.92f, 0.0f)				, 0.27 },
		{ glm::vec3(110.497f, 14.5025f, 12.1423f), glm::vec3(84.88f, -7.36001f, 0.0f)			, 0.28 },
		{ glm::vec3(110.558f, 15.6f, -18.3584f), glm::vec3(119.12f, -19.04f, 0.0f)				, 0.29 },
		{ glm::vec3(102.801f, 24.2902f, -54.9019f), glm::vec3(119.76f, -17.52f, 0.0f)			, 0.30 },
		{ glm::vec3(33.3217f, 24.2902f, -110.797f), glm::vec3(164.72f, -18.64f, 0.0f)			, 0.31 },
		{ glm::vec3(-34.7239f, 24.2902f, -114.123f), glm::vec3(198.801f, -17.28f, 0.0f)			, 0.32 },
		{ glm::vec3(-67.8184f, 20.52f, -72.105f), glm::vec3(244.161f, -14.96f, 0.0f)			, 0.33 },
		{ glm::vec3(-58.0856f, 16.3885f, -19.657f), glm::vec3(284.641f, -13.12f, 0.0f)			, 0.34 },
		{ glm::vec3(-23.5649f, 8.35816f, 8.85487f), glm::vec3(269.521f, -25.36f, 0.0f)			, 0.35 },
		{ glm::vec3(-2.68269f, -0.930763f, 17.7269f), glm::vec3(239.841f, -21.44f, 0.0f)		, 0.36 },
		{ glm::vec3(21.1806f, -9.55727f, 8.22238f), glm::vec3(352.961f, -11.28f, 0.0f)			, 0.37 },
		{ glm::vec3(12.647f, -13.2829f, -17.6487f), glm::vec3(386.881f, 13.6f, 0.0f)			, 0.38 },
		{ glm::vec3(-1.74525f, -0.896129f, -18.6995f), glm::vec3(512.801f, 36.8f, 0.0f)			, 1.00 }
	}
};

std::array<ControlPoint, 4> currentControlPoints;

std::array<int, 2> currentIndices;

glm::vec3 previousRotation;

AutomaticCamera::AutomaticCamera()
{
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