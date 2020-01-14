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
std::vector<std::vector<ControlPoint>> curves{
	//INTRO Using DirectSound8 driver






	{
		{ glm::vec3(6.08515f, -8.22027f, 528.232f), glm::vec3(-0.400001f, -2.80001f, 0.0f), 0.01 },
		{ glm::vec3(8.10513f, -10.6716f, 345.332f), glm::vec3(0.399999f, 1.67999f, 0.0f), 0.01 },
		{ glm::vec3(-12.9545f, -7.19209f, 190.49f), glm::vec3(-4.32f, -0.480012f, 0.0f), 0.01 },
		{ glm::vec3(5.79986f, -8.53529f, 99.6339f), glm::vec3(-29.84f, -1.92001f, 3.49124f), 0.01 },
		{ glm::vec3(44.0426f, -12.0314f, 48.8162f), glm::vec3(-41.92f, -4.08001f, -13.5041f), 0.01 },
		{ glm::vec3(63.9712f, -17.2962f, -23.0276f), glm::vec3(5.83998f, -3.76001f, -18.1175f), 0.015 },
		{ glm::vec3(48.4364f, -20.236f, -76.4254f), glm::vec3(98.48f, -1.20001f, -14.2328f), 0.015 },
		{ glm::vec3(6.19351f, -19.0798f, -95.4774f), glm::vec3(171.2f, -0.320021f, -5.9279f), 0.015 },
		{ glm::vec3(-58.5279f, -17.4675f, -92.4687f), glm::vec3(207.6f, 11.76f, -2.91814f), 0.015 }
	},
	//cube plops 
	{
		{ glm::vec3(-40.2953f, 9.17974f, 48.6856f), glm::vec3(-40.8001f, 8.71999f, 0.0f), 0.02 },
		{ glm::vec3(-41.3118f, 3.5167f, 97.002f), glm::vec3(-22.8001f, 3.67999f, 0.0f), 0.02 },
		{ glm::vec3(-26.0417f, 1.08776f, 135.801f), glm::vec3(-12.7201f, 1.51999f, 0.0f), 0.02 },
		{ glm::vec3(-22.2948f, -0.267128f, 198.856f), glm::vec3(-7.68007f, -0.720006f, 0.0f), 0.02 }
	},

	//front left to front right cube 
	{
		{ glm::vec3(-47.7308f, -5.14316f, 15.7767f), glm::vec3(-86.72f, -17.36f, 0.0f), 0.01 },
		{ glm::vec3(-25.3525f, -5.14316f, 54.0715f), glm::vec3(-36.56f, -17.52f, 0.0f), 0.01 },
		{ glm::vec3(28.9631f, -0.786949f, 81.6015f), glm::vec3(10.96f, -17.04f, 0.0f), 0.01 }
	},

	//move into block 
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

	//REFRAIN
	{
		{ glm::vec3(-13.1337f, 59.2527f, 292.999f), glm::vec3(-4.4f, -18.48f, 0.0f), 0.008},
		{ glm::vec3(-9.6444f, -4.08227f, 152.565f), glm::vec3(-6.24f, 14.24f, 0.0f), 0.01},
		{ glm::vec3(0.969842f, -9.40112f, 7.44251f), glm::vec3(-4.16f, -7.67999f, 0.0f), 0.01 },
		{ glm::vec3(27.3121f, -12.8691f, -77.1087f), glm::vec3(-22.4f, 2.80001f, 0.0f), 0.01 },
		{ glm::vec3(61.5336f, -7.16605f, -199.016f), glm::vec3(133.76f, -1.35999f, 0.0f),0.01 },
		{ glm::vec3(-152.683f, 21.7167f, -110.991f), glm::vec3(228.24f, -0.479988f, 0.0f),0.01 }
	},

	//slowmo starting from the donut - end at cupcake
	{
		{ glm::vec3(-38.3477f, -27.4734f, 120.91f), glm::vec3(-16.96f, 15.92f, 0.0f), 0.01 },
		{ glm::vec3(-36.8903f, -0.0790472f, 115.58f), glm::vec3(-20.32f, -1.20004f, 0.0f), 0.008 },
		{ glm::vec3(-10.012f, -0.0790472f, 122.102f), glm::vec3(-10.24f, -11.2f, 0.0f), 0.008 },
		{ glm::vec3(8.12148f, -9.00435f, 129.872f), glm::vec3(-15.44f, -29.04f, 0.0f), 0.008 }
	},

	//plattform firework
	{
		{ glm::vec3(-161.751f, 23.864f, 303.734f), glm::vec3(-30.2399f, -15.12f, 0.0f), 0.01 },
		{ glm::vec3(-23.0183f, -4.29861f, 282.244f), glm::vec3(-7.43995f, 0.800007f, 0.0f), 0.01 },
		{ glm::vec3(198.705f, 40.1072f, 234.853f), glm::vec3(36.96f, -14.88f, 0.0f), 0.005},
		{ glm::vec3(254.618f, 32.0461f, 113.232f), glm::vec3(64.1601f, -15.92f, 0.0f), 0.005 },
		{ glm::vec3(265.684f, 26.4661f, -30.7179f), glm::vec3(94.0802f, -14.88f, 0.0f), 0.005 },
		{ glm::vec3(164.984f, 10.2245f, -161.727f), glm::vec3(122.88f, -15.36f, 0.0f), 0.005 },
		{ glm::vec3(48.1583f, -18.6542f, -113.793f), glm::vec3(123.68f, -7.04003f, 0.0f),0.005 },
		{ glm::vec3(-17.6004f, -21.1718f, -63.9135f), glm::vec3(133.68f, 0.879973f, 0.0f), 0.005 },
		{ glm::vec3(-52.088f, -15.2322f, -21.9752f), glm::vec3(165.28f, 6.8f, 0.0f), 0.005},
		{ glm::vec3(-58.8637f, -5.48432f, 7.57474f), glm::vec3(188.961f, 25.2f, 0.0f), 0.005 },
		{ glm::vec3(-42.8258f, 4.6398f, 59.559f), glm::vec3(202.881f, 2.31999f, 0.0f), 0.007 },
		{ glm::vec3(39.4254f, -11.3497f, 104.0f), glm::vec3(298.961f, -16.16f, 0.0f), 0.009 },
		{ glm::vec3(117.94f, -21.3074f, 107.758f), glm::vec3(368.56f, -8.40002f, 0.0f), 0.01 }
	//	{ glm::vec3(155.182f, -23.3066f, 68.8224f), glm::vec3(416.48f, -8.64002f, 0.0f), 0.01 }
	//	{ glm::vec3(156.885f, -23.3066f, 11.7205f), glm::vec3(475.361f, -8.48002f, 0.0f),0.03 },
	},


	//move around the plattform
	{
		{ glm::vec3(153.133f, -39.1653f, -63.2707f), glm::vec3(-188.161f, -3.60006f, 0.0f), 0.01 },
		{ glm::vec3(177.052f, -39.049f, 45.6857f), glm::vec3(-230.561f, 3.43994f, 0.0f), 0.01 },
		{ glm::vec3(147.493f, -36.5853f, 122.185f), glm::vec3(-259.121f, 1.59994f, 0.0f),0.01 },
		{ glm::vec3(85.9365f, -34.8149f, 176.701f), glm::vec3(-291.681f, 1.83994f, 0.0f),0.01 },
		{ glm::vec3(11.6463f, -33.0544f, 186.622f), glm::vec3(-326.241f, 2.39994f, 0.0f),0.01 },
		{ glm::vec3(-34.8313f, -27.7101f, 146.888f), glm::vec3(-380.321f, 6.39994f, 0.0f), 0.01 }
	},

	//Camera is flying through the trees
	{
	{ glm::vec3(-6.94321f, 0.485368f, 197.678f), glm::vec3(32.08f, -5.92f, 0.0f), 0.005 },
	{ glm::vec3(-47.0274f, -5.8955f, 139.669f), glm::vec3(31.68f, -6.72f, 0.0f), 0.005},
	{ glm::vec3(-69.6741f, -9.40695f, 105.268f), glm::vec3(36.32f, -3.68f, 0.0f), 0.005 },
	{ glm::vec3(-113.337f, -9.50821f, 43.4124f), glm::vec3(26.88f, -0.480002f, 0.0f),0.005 },
	{ glm::vec3(-127.685f, -14.5517f, 8.37574f), glm::vec3(-10.96f, -10.08f, 0.0f), 0.005 },
	{ glm::vec3(-125.98f, -16.1919f, -0.768842f), glm::vec3(-10.56f, -9.92f, 0.0f), 0.005 },
	{ glm::vec3(-119.838f, -20.0307f, -23.1122f), glm::vec3(-27.04f, -3.36001f, 0.0f), 0.005 },
	{ glm::vec3(-106.893f, -22.2424f, -54.025f), glm::vec3(-51.52f, -4.16001f, 0.0f),0.005 },
	{ glm::vec3(-77.577f, -23.3673f, -76.1755f), glm::vec3(-72.56f, 1.91999f, 0.0f), 0.005 },
	{ glm::vec3(-60.1783f, -23.0558f, -84.9916f), glm::vec3(-127.76f, -0.480006f, 0.0f), 0.005 },
	{ glm::vec3(-45.9178f, -23.1922f, -72.4366f), glm::vec3(-146.32f, 1.19999f, 0.0f), 0.005 },
	{ glm::vec3(-38.1096f, -23.798f, -46.0384f), glm::vec3(-171.52f, -2.64001f, 0.0f), 0.005 },
	{ glm::vec3(-36.1522f, -24.5392f, -30.0908f), glm::vec3(-177.6f, -0.880007f, 0.0f), 0.005 },
	{ glm::vec3(-30.3064f, -22.4811f, -0.626716f), glm::vec3(-158.64f, 6.95999f, 0.0f), 0.005 },
	{ glm::vec3(-15.3982f, -20.1699f, 16.1421f), glm::vec3(-77.5199f, -0.0800079f, 0.0f), 0.005 }
//	{ glm::vec3(6.27734f, -15.468f, 7.26014f), glm::vec3(-18.9599f, 47.36f, 0.0f), 0.005 }
//	{ glm::vec3(12.0472f, 3.06162f, -10.524f), glm::vec3(-7.75988f, 52.0f, 0.0f), 0.01 },
//	{ glm::vec3(12.5048f, 17.9089f, -9.31594f), glm::vec3(72.9601f, 60.88f, 0.0f), 0.01 }
	},


			//Fly through the cube 

		{

			{ glm::vec3(143.823f, -38.3349f, -19.1295f), glm::vec3(98.8f, 21.04f, 0.0f), 0.01 },
			{ glm::vec3(80.6851f, -3.50753f, -9.93231f), glm::vec3(92.0f, 13.28f, 0.0f), 0.01 },
			{ glm::vec3(24.7471f, 4.72735f, -6.90856f), glm::vec3(92.16f, -0.879983f, 0.0f), 0.01 },
			{ glm::vec3(2.94069f, -5.66524f, -7.63382f), glm::vec3(88.0f, -27.6f, 0.0f), 0.01 },
			{ glm::vec3(-28.6745f, -16.4508f, -6.79653f), glm::vec3(93.36f, -12.96f, 0.0f), 0.01 },
			{ glm::vec3(-80.1064f, -20.8507f, 2.54302f), glm::vec3(104.48f, 0.960016f, 0.0f), 0.01 },
			{ glm::vec3(-165.013f, -31.4439f, 52.8426f), glm::vec3(224.24f, -16.72f, 0.0f), 0.01 },
			{ glm::vec3(-174.51f, -31.4439f, 119.99f), glm::vec3(289.04f, -5.19998f, 0.0f), 0.01 },
			{ glm::vec3(-139.31f, -31.4439f, 174.712f), glm::vec3(313.28f, 2.32002f, 0.0f), 0.01 }
		},

			//Move into Refrain 

	{
		{ glm::vec3(-285.5f, 112.152f, -172.666f), glm::vec3(-118.64f, -34.96f, 0.0f), 0.01 },
		{ glm::vec3(-162.247f, 29.4407f, -118.801f), glm::vec3(-110.08f, -26.48f, 0.0f), 0.01 },
		{ glm::vec3(-87.6807f, -9.91419f, -94.2373f), glm::vec3(-106.32f, -23.84f, 0.0f), 0.01 },
		{ glm::vec3(-48.434f, -20.6429f, -84.5817f), glm::vec3(-102.24f, -12.0f, 0.0f), 0.01 },
		{ glm::vec3(1.83566f, -17.0206f, -65.053f), glm::vec3(-99.36f, 16.88f, 0.0f), 0.01 },
		{ glm::vec3(28.73f, -16.3342f, -55.974f), glm::vec3(-56.88f, -12.4f, 0.0f), 0.01 },
		{ glm::vec3(37.2938f, -17.7274f, -63.1192f), glm::vec3(-44.4f, -8.23999f, 0.0f), 0.01 },
		{ glm::vec3(67.8845f, -21.6542f, -92.6114f), glm::vec3(-47.6f, 0.800006f, 0.0f), 0.005 },
		{ glm::vec3(104.223f, -20.6167f, -121.886f), glm::vec3(-147.28f, 3.04001f, 0.0f), 0.005 },
		{ glm::vec3(169.226f, -17.6476f, -87.5858f), glm::vec3(-230.4f, 6.32001f, 0.0f), 0.005 },
		{ glm::vec3(178.492f, -17.6476f, -75.1276f), glm::vec3(-249.2f, 8.88001f, 0.0f), 0.01 },
		{ glm::vec3(185.346f, -17.6476f, -57.0036f), glm::vec3(-250.88f, 7.60001f, 0.0f), 0.01 },
		{ glm::vec3(210.175f, 0.591212f, 57.6758f), glm::vec3(-284.32f, -0.879988f, 0.0f), 0.01 },
		{ glm::vec3(167.561f, 49.5643f, 247.831f), glm::vec3(-319.2f, -17.2f, 0.0f), 0.01 },
		{ glm::vec3(74.2086f, 13.1884f, 179.501f), glm::vec3(-327.36f, -17.04f, 0.0f), 0.01 },
		{ glm::vec3(41.3552f, -15.5436f, 110.912f), glm::vec3(-347.12f, -37.44f, 0.0f), 0.01 },
		{ glm::vec3(39.943f, -22.9573f, 104.624f), glm::vec3(21.76f, -55.92f, 0.0f), 0.01 },
		//{ glm::vec3(36.3981f, -18.6962f, 97.3912f), glm::vec3(-337.28f, -59.76f, 0.0f), 0.005 }

	},

	{
		{ glm::vec3(-2.72389f, -14.6622f, 105.158f), glm::vec3(-1.2517e-06f, 7.68f, 0.0f), 0.005 },
		{ glm::vec3(-57.4138f, -13.6443f, 76.502f), glm::vec3(-37.12f, 4.72f, 0.0f), 0.005 },
		{ glm::vec3(-74.1212f, -17.2914f, 9.68954f), glm::vec3(-82.96f, 8.08001f, 0.0f), 0.005 },
		{ glm::vec3(-72.8726f, -17.2914f, -24.4814f), glm::vec3(-105.36f, 12.4f, 0.0f), 0.005 },
		{ glm::vec3(-56.108f, -16.8019f, -54.2454f), glm::vec3(-128.24f, 9.60001f, 0.0f), 0.005 },
		{ glm::vec3(-38.0646f, -14.8848f, -55.7585f), glm::vec3(-142.0f, 21.2f, 0.0f), 0.005 },
		{ glm::vec3(5.13391f, -8.80207f, -57.1437f), glm::vec3(-185.92f, 8.32001f, 0.0f), 0.005 },
		{ glm::vec3(47.9249f, -10.1022f, -56.6511f), glm::vec3(-222.0f, -0.399989f, 0.0f), 0.005 },
		{ glm::vec3(59.8408f, -10.3697f, -29.1351f), glm::vec3(-245.92f, -2.23999f, 0.0f), 0.005 },
		{ glm::vec3(72.0859f, -14.7229f, 7.08671f), glm::vec3(-268.4f, -3.83999f, 0.0f), 0.006 },
		// over here problem too
		{ glm::vec3(55.6835f, -16.4478f, 35.5502f), glm::vec3(-300.4f, 2.16001f, 0.0f), 0.005 },
		{ glm::vec3(20.5944f, -15.2149f, 63.712f), glm::vec3(-339.6f, -2.71999f, 0.0f), 0.005 },
		{ glm::vec3(-5.69401f, -15.2149f, 68.2888f), glm::vec3(-367.04f, 19.92f, 0.0f), 0.005 },
		//problem here
		{ glm::vec3(-30.118f, -15.2149f, 55.014f), glm::vec3(-389.76f, 12.4f, 0.0f), 0.006 },
		{ glm::vec3(-54.7506f, -15.2149f, 46.8998f), glm::vec3(-411.44f, 10.24f, 0.0f), 0.006 },
		{ glm::vec3(-66.4957f, -15.2149f, 30.9581f), glm::vec3(-427.12f, 8.64001f, 0.0f), 0.006 },
		{ glm::vec3(-71.023f, -15.2149f, -17.3848f), glm::vec3(-463.36f, 15.52f, 0.0f), 0.006 },
		{ glm::vec3(-64.7651f, -15.2149f, -51.9457f), glm::vec3(-480.8f, 7.36001f, 0.0f), 0.006 },
		{ glm::vec3(-5.00028f, -20.4705f, -71.5721f), glm::vec3(-532.96f, 12.72f, 0.0f), 0.006 },
		{ glm::vec3(26.8509f, -21.0842f, -77.2059f), glm::vec3(-562.88f, 19.52f, 0.0f), 0.007 },
		{ glm::vec3(54.8638f, -21.0687f, -71.5633f), glm::vec3(-574.08f, 2.56001f, 0.0f), 0.006 },
		{ glm::vec3(70.7992f, -20.8226f, -46.2411f), glm::vec3(-599.6f, 14.32f, 0.0f), 0.006 },
		{ glm::vec3(79.2758f, -18.6411f, -5.7982f), glm::vec3(-630.88f, 12.24f, 0.0f), 0.006 },
		{ glm::vec3(62.1009f, -16.4871f, 19.7019f), glm::vec3(-655.281f, 7.04001f, 0.0f), 0.007 },
		{ glm::vec3(46.8831f, -15.8444f, 48.3734f), glm::vec3(-679.921f, 14.16f, 0.0f), 0.007 },
		{ glm::vec3(22.8035f, -20.2911f, 88.6109f), glm::vec3(-707.601f, 10.08f, 0.0f), 0.006 },
		{ glm::vec3(-2.18918f, -20.3176f, 97.9642f), glm::vec3(-723.921f, 13.68f, 0.0f), 0.01 },
		{ glm::vec3(-14.6475f, -19.806f, 145.923f), glm::vec3(-725.041f, 4.32001f, 0.0f), 0.01 },
		{ glm::vec3(-20.2627f, -23.9129f, 193.8f), glm::vec3(-727.041f, 6.48001f, 0.0f), 0.02 },
		{ glm::vec3(-26.0918f, -29.6987f, 244.951f), glm::vec3(-726.081f, 6.32001f, 0.0f), 0.03 },
		{ glm::vec3(-35.131f, -38.344f, 324.159f), glm::vec3(-726.88f, 6.08001f, 0.0f), 0.04 },
		{ glm::vec3(-41.2894f, -43.8199f, 375.199f), glm::vec3(-726.88f, 6.08001f, 0.0f), 0.04 },
		{ glm::vec3(-47.7632f, -45.3637f, 423.908f), glm::vec3(-727.84f, -0.399993f, 0.0f), 0.04 },
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