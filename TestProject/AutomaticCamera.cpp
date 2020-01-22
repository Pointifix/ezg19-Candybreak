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
	//scene introduction - start scene
	{
		{ glm::vec3(-42.887f, 6.913f, 420.643f), glm::vec3(-11.840f, -4.800f, 0.000f), 0.009 },
		{ glm::vec3(-11.563f, -7.743f, 223.847f), glm::vec3(-4.000f, -3.200f, 0.000f), 0.009 },
		{ glm::vec3(-7.864f, -12.453f, 96.857f), glm::vec3(-37.840f, 0.080f, 0.000f), 0.009 },
		{ glm::vec3(50.937f, -15.327f, 39.500f), glm::vec3(2.640f, -4.960f, 0.000f), 0.009 },
		{ glm::vec3(68.753f, -21.295f, -41.581f), glm::vec3(40.160f, -1.920f, 0.000f), 0.009 },
		{ glm::vec3(33.086f, -23.202f, -91.753f), glm::vec3(85.520f, 3.920f, 0.000f), 0.009 },
		{ glm::vec3(-10.204f, -21.051f, -95.435f), glm::vec3(131.440f, 1.680f, 0.000f), 0.009 },
		{ glm::vec3(-58.224f, -18.234f, -78.390f), glm::vec3(205.360f, -0.560f, 0.000f), 0.009 },
	}, //length: 0.063 - current progress: 0.063
	//scene showcase models (donut, muffin, candy cane, tress)
	{
		{ glm::vec3(-129.045f, 16.904f, -123.879f), glm::vec3(-150.640f, -22.800f, 0.000f), 0.008 },
		{ glm::vec3(-81.408f, -11.074f, -40.582f), glm::vec3(-206.720f, -16.560f, 0.000f), 0.008 },
		{ glm::vec3(-100.788f, -18.270f, 17.030f), glm::vec3(-112.000f, -5.040f, 0.000f), 0.008 },
		{ glm::vec3(-57.717f, -17.014f, 64.627f), glm::vec3(-59.760f, -2.560f, 0.000f), 0.008 },
		{ glm::vec3(17.401f, -12.806f, 100.828f), glm::vec3(-18.480f, 1.600f, 0.000f), 0.008 },
		{ glm::vec3(81.261f, -13.335f, 65.645f), glm::vec3(39.280f, -4.160f, 0.000f), 0.008 },
		{ glm::vec3(159.231f, -1.648f, 45.038f), glm::vec3(75.280f, -13.520f, 0.000f), 0.008 },
	}, //length: 0.048 - current progress: 0.111
	//scene bricks spawning in
	{
		{ glm::vec3(7.107f, 11.618f, 92.165f), glm::vec3(2.160f, -0.880f, 0.000f), 0.008 },
		{ glm::vec3(-4.092f, -12.900f, 117.868f), glm::vec3(-4.480f, 10.480f, 0.000f), 0.008 },
		{ glm::vec3(-9.544f, -20.230f, 177.202f), glm::vec3(-5.680f, 4.400f, 0.000f), 0.008 },
		{ glm::vec3(-15.790f, -20.884f, 235.301f), glm::vec3(-6.560f, -2.320f, 0.000f), 0.008 },
		{ glm::vec3(-60.956f, -17.828f, 266.379f), glm::vec3(-15.360f, -6.160f, 0.000f), 0.008 },
		{ glm::vec3(-103.450f, -21.806f, 198.357f), glm::vec3(-36.240f, -3.280f, 0.000f), 0.008 },
		{ glm::vec3(-108.026f, -24.147f, 120.986f), glm::vec3(-55.200f, 0.880f, 0.000f), 0.008 },
		{ glm::vec3(-81.256f, -21.775f, 67.264f), glm::vec3(-62.080f, 4.880f, 0.000f), 0.008 },
		{ glm::vec3(-53.229f, -18.056f, 39.961f), glm::vec3(-33.360f, 0.960f, 0.000f), 0.008 },
		{ glm::vec3(-41.099f, -18.662f, 8.089f), glm::vec3(2.880f, -1.840f, 0.000f), 0.008 },
		{ glm::vec3(-52.986f, -18.258f, -33.640f), glm::vec3(-62.960f, 1.840f, 0.000f), 0.008 },
		{ glm::vec3(-22.476f, -17.901f, -70.422f), glm::vec3(-149.520f, -0.560f, 0.000f), 0.008 },
		{ glm::vec3(9.623f, -17.895f, -104.140f), glm::vec3(-184.000f, 0.240f, 0.000f), 0.008 },
		{ glm::vec3(43.251f, -17.895f, -93.723f), glm::vec3(-213.680f, -0.880f, 0.000f), 0.008 },
	}, //length: 0.104 - current progress: 0.215
	//scene showcase inner side of cube / ball starts bouncing
	{
		{ glm::vec3(70.704f, -21.144f, -109.557f), glm::vec3(-206.081f, -4.000f, 0.000f), 0.008 },
		{ glm::vec3(50.202f, -21.644f, -57.198f), glm::vec3(-195.120f, 2.640f, 0.000f), 0.008 },
		{ glm::vec3(48.543f, -19.375f, -2.994f), glm::vec3(-265.120f, 2.720f, 0.000f), 0.008 },
		{ glm::vec3(42.689f, -10.868f, 55.973f), glm::vec3(-326.320f, -12.640f, 0.000f), 0.008 },
		{ glm::vec3(13.487f, -1.267f, 101.492f), glm::vec3(-352.641f, -14.560f, 0.000f), 0.008 },
		{ glm::vec3(-33.116f, 13.788f, 154.274f), glm::vec3(-374.640f, -14.960f, 0.000f), 0.007 },
		{ glm::vec3(-124.574f, 33.521f, 171.432f), glm::vec3(-402.720f, -18.400f, 0.000f), 0.007 },
		{ glm::vec3(-199.020f, 50.362f, 138.635f), glm::vec3(-425.280f, -21.520f, 0.000f), 0.007 },
		{ glm::vec3(-187.007f, 36.271f, 64.273f), glm::vec3(-439.600f, -21.280f, 0.000f), 0.007 },
		{ glm::vec3(-132.778f, 12.193f, 7.997f), glm::vec3(-453.519f, -11.680f, 0.000f), 0.007 },
		{ glm::vec3(-73.056f, -4.728f, -10.919f), glm::vec3(-463.839f, -2.320f, 0.000f), 0.007 },
		{ glm::vec3(-14.923f, -11.370f, -19.461f), glm::vec3(-489.759f, -2.560f, 0.000f), 0.007 },
	}, //length: 0.082 - current progress: 0.297
	// scene first beat - first explosion
	{
		{ glm::vec3(117.778f, 45.621f, -83.335f), glm::vec3(-241.200f, -41.680f, 0.000f), 0.003 },
		{ glm::vec3(69.588f, 45.621f, -130.278f), glm::vec3(-209.360f, -29.680f, 0.000f), 0.003 },
		{ glm::vec3(-22.929f, 49.372f, -157.833f), glm::vec3(-174.320f, -24.000f, 0.000f), 0.003 },
		{ glm::vec3(-102.970f, 76.955f, -189.756f), glm::vec3(-150.320f, -26.080f, 0.000f), 0.003 },
		{ glm::vec3(-198.442f, 107.153f, -175.963f), glm::vec3(-131.439f, -30.880f, 0.000f), 0.003 },
		{ glm::vec3(-202.084f, 68.854f, -71.032f), glm::vec3(-116.319f, -31.120f, 0.000f), 0.003 },
		{ glm::vec3(-153.667f, 31.332f, 9.277f), glm::vec3(-82.959f, -27.120f, 0.000f), 0.003 },
		{ glm::vec3(-79.335f, 9.271f, 40.303f), glm::vec3(-57.839f, -16.560f, 0.000f), 0.003 },
		{ glm::vec3(-18.273f, -1.777f, 43.401f), glm::vec3(-23.839f, -11.600f, 0.000f), 0.003 },
		{ glm::vec3(23.176f, -10.072f, 1.949f), glm::vec3(39.521f, -8.080f, 0.000f), 0.003 },
		{ glm::vec3(44.554f, -7.690f, -27.534f), glm::vec3(104.961f, -10.720f, 0.000f), 0.003 },
		{ glm::vec3(16.250f, -5.781f, -59.125f), glm::vec3(158.881f, -8.240f, 0.000f), 0.003 },
		{ glm::vec3(-19.183f, -10.017f, -32.442f), glm::vec3(196.961f, -6.560f, 0.000f), 0.003 },
		{ glm::vec3(-24.182f, -8.240f, 0.362f), glm::vec3(222.801f, 17.360f, 0.000f), 0.003 },
		{ glm::vec3(-20.027f, -2.932f, 27.346f), glm::vec3(282.241f, 13.600f, 0.000f), 0.003 },
		{ glm::vec3(-3.199f, -0.324f, 93.491f), glm::vec3(345.041f, -9.520f, 0.000f), 0.003 },
	}, //length: 0.045 - current progress: 0.342
	{
		{ glm::vec3(-128.188f, 87.116f, 172.014f), glm::vec3(316.480f, -33.120f, 0.000f), 0.004 },
		{ glm::vec3(-118.035f, 40.760f, 60.531f), glm::vec3(292.080f, -29.840f, 0.000f), 0.004 },
		{ glm::vec3(-63.221f, 11.960f, 4.030f), glm::vec3(259.521f, -27.120f, 0.000f), 0.004 },
		{ glm::vec3(-12.563f, -7.736f, -21.199f), glm::vec3(229.201f, -20.960f, 0.000f), 0.004 },
		{ glm::vec3(30.972f, -15.436f, -15.518f), glm::vec3(170.081f, -6.240f, 0.000f), 0.004 },
		{ glm::vec3(75.867f, -12.282f, -2.111f), glm::vec3(115.201f, -12.160f, 0.000f), 0.004 },
		{ glm::vec3(145.439f, 17.289f, 24.329f), glm::vec3(74.081f, -24.560f, 0.000f), 0.004 },
		{ glm::vec3(140.786f, 41.127f, 101.524f), glm::vec3(40.561f, -23.200f, 0.000f), 0.004 },
		{ glm::vec3(59.347f, 77.026f, 208.664f), glm::vec3(5.361f, -27.360f, 0.000f), 0.004 },
	}, //length: 0.032 - current progress: 0.374
	{
		{ glm::vec3(-35.579f, 34.244f, 162.145f), glm::vec3(-22.080f, -18.320f, 0.000f), 0.005 },
		{ glm::vec3(-147.896f, 46.066f, 58.625f), glm::vec3(-73.600f, -19.440f, 0.000f), 0.005 },
		{ glm::vec3(-102.699f, 52.620f, -114.162f), glm::vec3(-142.480f, -21.760f, 0.000f), 0.005 },
		{ glm::vec3(14.866f, 60.229f, -160.533f), glm::vec3(-186.801f, -25.520f, 0.000f), 0.005 },
		{ glm::vec3(23.255f, 79.325f, -199.057f), glm::vec3(-187.201f, -26.160f, 0.000f), 0.005 },
	}, //length: 0.02 - current progress: 0.394
	{
		{ glm::vec3(39.446f, -11.390f, 154.080f), glm::vec3(1.120f, -2.480f, 0.000f), 0.004 },
		{ glm::vec3(26.796f, -13.492f, 90.774f), glm::vec3(25.520f, -0.080f, 0.000f), 0.004 },
		{ glm::vec3(-5.053f, -14.337f, 44.490f), glm::vec3(28.400f, -2.480f, 8.504f), 0.004 },
		{ glm::vec3(-24.134f, -12.952f, 12.767f), glm::vec3(-14.880f, -4.320f, 16.909f), 0.004 },
		{ glm::vec3(-21.763f, -10.068f, -18.823f), glm::vec3(-82.320f, -4.480f, 22.309f), 0.004 },
		{ glm::vec3(1.500f, 2.697f, -45.218f), glm::vec3(-187.520f, -15.040f, 43.988f), 0.004 },
		{ glm::vec3(11.424f, 32.479f, -96.343f), glm::vec3(-218.160f, -41.200f, 99.316f), 0.004 },
		{ glm::vec3(25.515f, 50.508f, -170.706f), glm::vec3(-222.640f, -42.720f, 169.112f), 0.004 },
		{ glm::vec3(30.094f, 71.477f, -262.957f), glm::vec3(-213.760f, -49.200f, 296.344f), 0.004 },
		{ glm::vec3(44.254f, 101.760f, -369.833f), glm::vec3(-215.200f, -51.360f, 352.428f), 0.004 },
	}, //length: 0.036 - current progress: 0.430
	{
		{ glm::vec3(-58.084f, 28.369f, 179.801f), glm::vec3(-27.360f, -20.560f, 0.000f), 0.004 },
		{ glm::vec3(12.888f, 7.419f, 122.241f), glm::vec3(7.200f, -16.400f, 0.000f), 0.004 },
		{ glm::vec3(36.586f, -11.227f, 34.439f), glm::vec3(53.040f, -6.240f, 0.000f), 0.004 },
		{ glm::vec3(31.715f, -12.968f, -7.671f), glm::vec3(99.920f, -4.000f, 0.000f), 0.004 },
		{ glm::vec3(-5.037f, -10.086f, -57.317f), glm::vec3(176.080f, -10.720f, 0.000f), 0.004 },
		{ glm::vec3(-13.827f, 2.903f, -107.896f), glm::vec3(184.880f, -15.600f, 0.000f), 0.004 },
		{ glm::vec3(-77.705f, 25.188f, -139.171f), glm::vec3(216.640f, -22.160f, 0.000f), 0.004 },
		{ glm::vec3(-168.997f, 47.988f, -81.645f), glm::vec3(254.881f, -23.360f, 0.000f), 0.004 },
		{ glm::vec3(-143.308f, 39.285f, 17.486f), glm::vec3(290.400f, -22.880f, 0.000f), 0.005 },
		{ glm::vec3(-81.970f, 19.754f, 39.181f), glm::vec3(316.800f, -28.000f, 0.000f), 0.005 },
		{ glm::vec3(-34.688f, -3.950f, 6.639f), glm::vec3(261.200f, -16.480f, 0.000f), 0.005 },
		{ glm::vec3(1.725f, -8.106f, -24.952f), glm::vec3(191.520f, -9.760f, 0.000f), 0.005 },
		{ glm::vec3(26.054f, 3.542f, -66.275f), glm::vec3(170.000f, -19.360f, 0.000f), 0.005 },
		{ glm::vec3(59.337f, 21.680f, -103.655f), glm::vec3(154.560f, -22.880f, 0.000f), 0.005 },
		{ glm::vec3(134.088f, 47.309f, -104.251f), glm::vec3(125.040f, -28.560f, 0.000f), 0.005 },
		{ glm::vec3(194.612f, 71.737f, -39.420f), glm::vec3(91.280f, -29.200f, 0.000f), 0.005 },
	}, //length: 0.067 - current progress: 0.497
	{
		{ glm::vec3(10.795f, -15.886f, 153.157f), glm::vec3(-4.959f, -4.160f, 0.000f), 0.008 },
		{ glm::vec3(7.814f, -15.769f, 116.814f), glm::vec3(18.081f, 5.520f, 0.000f), 0.008 },
		{ glm::vec3(-0.469f, -13.452f, 76.690f), glm::vec3(-3.919f, -1.360f, 0.000f), 0.008 },
		{ glm::vec3(-12.396f, -14.083f, 35.115f), glm::vec3(-41.359f, -2.000f, 0.000f), 0.008 },
		{ glm::vec3(-10.785f, -14.299f, -15.122f), glm::vec3(-128.799f, -2.880f, 0.000f), 0.008 },
		{ glm::vec3(14.616f, -6.048f, -61.562f), glm::vec3(-207.440f, -18.400f, 0.000f), 0.008 },
		{ glm::vec3(77.129f, 22.090f, -63.695f), glm::vec3(-244.560f, -25.280f, 0.000f), 0.008 },
		{ glm::vec3(135.959f, 48.336f, -1.699f), glm::vec3(-274.879f, -25.840f, 0.000f), 0.008 },
		{ glm::vec3(136.662f, 81.444f, 92.284f), glm::vec3(-314.159f, -32.560f, 0.000f), 0.008 },
	}, //length: 0.064 - current progress: 0.561
	{
		{ glm::vec3(8.609f, 65.149f, 77.632f), glm::vec3(-363.679f, -54.800f, 0.000f), 0.008 },
		{ glm::vec3(11.319f, -6.490f, 27.591f), glm::vec3(-359.919f, -26.480f, 0.000f), 0.008 },
		{ glm::vec3(12.830f, -15.845f, 0.432f), glm::vec3(-367.039f, 45.120f, 0.000f), 0.008 },
		{ glm::vec3(15.595f, -7.294f, -11.171f), glm::vec3(-303.119f, 68.400f, 13.693f), 0.008 },
		{ glm::vec3(13.372f, 10.473f, -13.514f), glm::vec3(-215.439f, 86.640f, 34.315f), 0.008 },
		{ glm::vec3(10.804f, 31.611f, -12.904f), glm::vec3(-221.919f, 46.000f, 52.699f), 0.008 },
		{ glm::vec3(16.870f, 63.516f, -13.532f), glm::vec3(-348.559f, -85.280f, 63.354f), 0.008 },
		{ glm::vec3(34.731f, 144.227f, -18.203f), glm::vec3(-380.239f, -95.760f, -12.201f), 0.008 },
		{ glm::vec3(58.280f, 266.480f, -23.168f), glm::vec3(-380.239f, -95.840f, -12.201f), 0.008 },
	}, //length: 0.064 - current progress: 0.625
	{
		{ glm::vec3(9.417f, -11.829f, 89.215f), glm::vec3(11.600f, 356.880f, 0.000f), 0.007 },
		{ glm::vec3(22.717f, -13.815f, 39.917f), glm::vec3(59.280f, 357.840f, 0.000f), 0.007 },
		{ glm::vec3(13.527f, -13.144f, -0.718f), glm::vec3(150.000f, 356.480f, 0.000f), 0.007 },
		{ glm::vec3(-17.946f, -9.986f, -42.010f), glm::vec3(213.360f, 356.160f, 0.000f), 0.007 },
		{ glm::vec3(-56.216f, -6.209f, -1.396f), glm::vec3(278.160f, 350.880f, 0.000f), 0.007 },
		{ glm::vec3(-72.017f, 6.771f, 62.822f), glm::vec3(316.640f, 342.640f, 0.000f), 0.007 },
		{ glm::vec3(-15.897f, 24.974f, 141.804f), glm::vec3(356.320f, 340.720f, 0.000f), 0.007 },
		{ glm::vec3(78.506f, 42.063f, 161.869f), glm::vec3(388.640f, 338.960f, 0.000f), 0.007 },
		{ glm::vec3(162.398f, 50.496f, 101.317f), glm::vec3(420.080f, 336.641f, 0.000f), 0.007 },
		{ glm::vec3(120.215f, 21.191f, 19.687f), glm::vec3(455.680f, 333.441f, 0.000f), 0.007 },
		{ glm::vec3(56.553f, -2.285f, -11.183f), glm::vec3(481.360f, 342.160f, 0.000f), 0.007 },
		{ glm::vec3(14.612f, -13.223f, 21.861f), glm::vec3(398.960f, 352.880f, 0.000f), 0.007 },
		{ glm::vec3(-26.055f, -3.294f, 70.594f), glm::vec3(344.961f, 343.361f, 0.000f), 0.007 },
		{ glm::vec3(-136.204f, 41.603f, 114.978f), glm::vec3(307.681f, 337.361f, 0.000f), 0.007 },
	}, //length: 0.091 - current progress: 0.716
	{
		{ glm::vec3(26.540f, -9.090f, 42.460f), glm::vec3(16.000f, -10.960f, 0.000f), 0.004 },
		{ glm::vec3(42.989f, -11.034f, 11.355f), glm::vec3(71.760f, -4.000f, 0.000f), 0.004 },
		{ glm::vec3(19.540f, -11.327f, -15.842f), glm::vec3(121.440f, 19.520f, 0.000f), 0.004 },
		{ glm::vec3(-27.381f, -20.372f, -42.954f), glm::vec3(218.560f, 21.840f, 0.000f), 0.004 },
		{ glm::vec3(-56.636f, -15.529f, -41.290f), glm::vec3(240.880f, 0.160f, 0.000f), 0.004 },
		{ glm::vec3(-64.029f, -16.175f, 30.808f), glm::vec3(309.280f, 1.520f, 0.000f), 0.004 },
		{ glm::vec3(-61.475f, -11.008f, 86.590f), glm::vec3(331.520f, -6.880f, 0.000f), 0.005 },
		{ glm::vec3(-19.238f, -8.973f, 118.329f), glm::vec3(348.080f, -7.600f, 0.000f), 0.004 },
	}, //length: 0.029 - current progress: 0.745
	{
		{ glm::vec3(-94.293f, 23.921f, 57.275f), glm::vec3(299.280f, -18.880f, 0.000f), 0.005 },
		{ glm::vec3(-64.741f, 76.902f, 199.967f), glm::vec3(339.039f, -30.080f, 0.000f), 0.005 },
		{ glm::vec3(128.454f, 97.019f, 183.146f), glm::vec3(395.279f, -32.000f, 0.000f), 0.005 },
		{ glm::vec3(226.217f, 100.576f, 13.944f), glm::vec3(445.518f, -31.280f, 0.000f), 0.005 },
		{ glm::vec3(148.051f, 87.395f, -135.628f), glm::vec3(492.558f, -32.480f, 0.000f), 0.005 },
		{ glm::vec3(11.479f, 69.800f, -169.356f), glm::vec3(536.238f, -31.600f, 0.000f), 0.005 },
		{ glm::vec3(-53.029f, 40.837f, -101.687f), glm::vec3(570.477f, -25.200f, 0.000f), 0.005 },
	}, //length: 0.03 - current progress: 0.775
	{
		{ glm::vec3(-35.598f, -16.355f, -11.161f), glm::vec3(-45.600f, -0.560f, 0.000f), 0.004 },
		{ glm::vec3(-39.170f, 10.219f, 41.767f), glm::vec3(-7.760f, -32.560f, 0.000f), 0.004 },
		{ glm::vec3(35.881f, 50.994f, 58.172f), glm::vec3(57.040f, -45.840f, 0.000f), 0.004 },
		{ glm::vec3(80.281f, 120.387f, -22.029f), glm::vec3(118.960f, -60.080f, 0.000f), 0.004 },
		{ glm::vec3(32.184f, 196.408f, -94.690f), glm::vec3(168.160f, -67.200f, 0.000f), 0.004 },
		{ glm::vec3(-80.971f, 303.453f, -85.644f), glm::vec3(232.720f, -69.840f, 0.000f), 0.003 },
		{ glm::vec3(-90.503f, 252.417f, 8.010f), glm::vec3(270.800f, -75.600f, -33.433f), 0.003 },
		{ glm::vec3(-33.434f, 180.055f, 16.277f), glm::vec3(361.600f, -84.080f, -57.578f), 0.003 },
		{ glm::vec3(-15.358f, 101.486f, 23.589f), glm::vec3(412.400f, -86.800f, -67.199f), 0.003 },
		{ glm::vec3(-10.910f, 24.359f, 11.851f), glm::vec3(462.320f, -77.440f, -103.727f), 0.003 },
		{ glm::vec3(-3.148f, -7.234f, 16.327f), glm::vec3(509.440f, -20.880f, -128.289f), 0.003 },
		{ glm::vec3(16.415f, -14.785f, 12.456f), glm::vec3(566.240f, 8.400f, -131.359f), 0.003 },
		{ glm::vec3(47.874f, -4.562f, 37.364f), glm::vec3(641.600f, -5.520f, -131.359f), 0.003 },
		{ glm::vec3(74.296f, 4.446f, 69.414f), glm::vec3(639.360f, -9.040f, -125.116f), 0.003 },
		{ glm::vec3(124.197f, 53.700f, 136.874f), glm::vec3(634.000f, -21.520f, -121.587f), 0.003 },
		{ glm::vec3(119.741f, 101.780f, 220.063f), glm::vec3(620.880f, -18.240f, -121.587f), 0.003 },
	}, //length: 0.05 - current progress: 0.825
	{
		{ glm::vec3(-88.444f, -10.089f, 29.536f), glm::vec3(-64.480f, -6.480f, 0.000f), 0.004 },
		{ glm::vec3(-60.075f, -15.246f, 3.525f), glm::vec3(-79.520f, -4.240f, 0.000f), 0.004 },
		{ glm::vec3(-18.057f, -11.370f, -23.135f), glm::vec3(-142.800f, -4.320f, 11.744f), 0.004 },
		{ glm::vec3(35.926f, 11.713f, -63.632f), glm::vec3(-230.640f, -23.760f, 26.379f), 0.004 },
		{ glm::vec3(68.783f, 29.393f, -17.530f), glm::vec3(-372.880f, -94.240f, 73.638f), 0.004 },
		{ glm::vec3(93.946f, 34.838f, 16.699f), glm::vec3(-484.320f, 3.680f, 183.492f), 0.004 },
		{ glm::vec3(54.653f, 54.858f, 99.216f), glm::vec3(-542.800f, 7.360f, 194.335f), 0.004 },
		{ glm::vec3(-41.641f, 75.403f, 117.448f), glm::vec3(-582.720f, 5.440f, 194.335f), 0.004 },
		{ glm::vec3(-83.106f, 65.890f, 43.949f), glm::vec3(-623.280f, -10.720f, 209.307f), 0.004 },
		{ glm::vec3(-42.147f, 27.905f, -14.783f), glm::vec3(-668.239f, -6.400f, 218.691f), 0.004 },
		{ glm::vec3(-19.637f, 8.421f, -11.442f), glm::vec3(-650.719f, 12.640f, 213.582f), 0.004 },
		{ glm::vec3(13.038f, -6.194f, -9.491f), glm::vec3(-719.919f, 17.520f, 219.213f), 0.004 },
		{ glm::vec3(65.165f, 12.743f, -10.138f), glm::vec3(-821.759f, 17.520f, 203.673f), 0.004 },
		{ glm::vec3(109.572f, 44.824f, 31.546f), glm::vec3(-846.079f, 10.640f, 203.673f), 0.004 },
	}, //length: 0.052 - current progress: 0.877
	{
		{ glm::vec3(-60.520f, 26.922f, 129.392f), glm::vec3(-37.280f, -17.120f, 0.000f), 0.005 },
		{ glm::vec3(-56.133f, 11.746f, 30.155f), glm::vec3(-76.320f, -7.040f, 0.000f), 0.005 },
		{ glm::vec3(-19.486f, 8.540f, 12.104f), glm::vec3(-97.360f, -29.600f, 0.000f), 0.005 },
		{ glm::vec3(-7.593f, -5.795f, 13.464f), glm::vec3(-95.920f, -61.280f, 0.000f), 0.005 },
		{ glm::vec3(8.296f, -18.906f, 19.948f), glm::vec3(-27.280f, -11.280f, 0.000f), 0.005 },
		{ glm::vec3(25.575f, -7.462f, 59.906f), glm::vec3(24.000f, -14.720f, 0.000f), 0.005 },
		{ glm::vec3(78.330f, 2.565f, 64.214f), glm::vec3(56.320f, -15.360f, 0.000f), 0.005 },
		{ glm::vec3(138.707f, 14.667f, 23.365f), glm::vec3(94.480f, -15.600f, 0.000f), 0.005 },
		{ glm::vec3(127.632f, 14.478f, -39.177f), glm::vec3(104.960f, -17.760f, 0.000f), 0.005 },
		{ glm::vec3(74.129f, -3.063f, -34.833f), glm::vec3(139.600f, -18.160f, 0.000f), 0.005 },
		{ glm::vec3(46.744f, -11.324f, 1.628f), glm::vec3(76.160f, -7.600f, 0.000f), 0.006 },
		{ glm::vec3(1.990f, -7.677f, 15.090f), glm::vec3(20.640f, 32.880f, 0.000f), 0.006 },
		{ glm::vec3(-17.669f, 3.990f, 8.837f), glm::vec3(-28.240f, 69.040f, 0.000f), 0.007 },
		{ glm::vec3(-15.273f, 19.449f, 8.439f), glm::vec3(-103.280f, 77.120f, 0.000f), 0.007 },
		{ glm::vec3(-8.391f, 57.441f, 6.597f), glm::vec3(-72.800f, -82.960f, 0.000f), 0.007 },
		{ glm::vec3(4.449f, 96.693f, -13.580f), glm::vec3(-185.760f, -79.440f, 0.000f), 0.007 },
		{ glm::vec3(13.752f, 140.783f, 17.478f), glm::vec3(-338.160f, -80.960f, 0.000f), 0.007 },
		{ glm::vec3(-14.419f, 193.011f, -6.197f), glm::vec3(-472.240f, -88.080f, 0.000f), 0.008 },
		{ glm::vec3(3.485f, 278.572f, 1.144f), glm::vec3(-653.440f, -90.720f, 0.000f), 0.008 },
		{ glm::vec3(0.595f, 362.922f, -4.760f), glm::vec3(-819.920f, -91.840f, 0.000f), 0.01 },
		{ glm::vec3(0.404f, 426.816f, -3.683f), glm::vec3(-962.480f, -92.800f, 0.000f), 0.001 },
	}, //length: 0.123 - current progress: 1.0
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
	this->rotation = glm::vec3(0.0f);
	this->up = this->UP;
	this->front = this->FRONT;
	this->right = this->RIGHT;

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
		curve.insert(curve.begin(), { glm::vec3(add.position), glm::vec3(add.rotation), add.t });
		add = curve.at(curve.size() - 1);
		curve.push_back({ glm::vec3(add.position), glm::vec3(add.rotation), add.t });
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
			global::camera->rotation = glm::vec3(0.0f);
			global::camera->up = global::camera->UP;
			global::camera->front = global::camera->FRONT;
			global::camera->right = global::camera->RIGHT;
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

	this->position = glm::catmullRom(
		currentControlPoints[0].position, 
		currentControlPoints[1].position, 
		currentControlPoints[2].position, 
		currentControlPoints[3].position, localT);
	
	glm::vec3 newRotation = glm::catmullRom(
		currentControlPoints[0].rotation, 
		currentControlPoints[1].rotation,
		currentControlPoints[2].rotation,
		currentControlPoints[3].rotation, localT);

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