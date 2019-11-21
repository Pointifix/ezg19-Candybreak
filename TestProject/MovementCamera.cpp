#include "MovementCamera.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

MovementCamera::MovementCamera()
{
}


MovementCamera::~MovementCamera()
{
}


float tmpYaw = 0.0f;
float tmpPitch = 0.0f;
std::vector<glm::vec3>* pos;
std::vector<glm::vec3>* rotation;

void MovementCamera::init() {
	//x, y, z
	glm::vec3 pos1 = glm::vec3(0.0f, 0.0f, 100.0f);
	//yaw, roll, pitch
	glm::vec3 rot1 = glm::vec3(0.0f, 0.0f, 0.0f);


	//rechts
	glm::vec3 pos2 = glm::vec3(53.0f, 0.04f, 72.3f);
	glm::vec3 rot2 = glm::vec3(34.5f, 0.0f, -0.16f);
	//links
	glm::vec3 pos3 = glm::vec3(-58.0f, 0.04f, 56.31f);
	glm::vec3 rot3 = glm::vec3(-46.5f,0.0f, -2.64f);
	//oben
	glm::vec3 pos4 = glm::vec3(-8.8f, 32.01f, 38.76f);
	glm::vec3 rot4 = glm::vec3(12.f, 0.f, -33.12f);
	//unten
	glm::vec3 pos5 = glm::vec3(1.07f, -36.0f, 15.0f);
	glm::vec3 rot5 = glm::vec3(-2.73f, 0, 62.64f);
	glm::vec3 pos6 = glm::vec3(1.07f, -36.0f, 15.0f);
	glm::vec3 rot6 = glm::vec3(-2.73f, 0, 62.64f);
	glm::vec3 pos7 = glm::vec3(1.07f, -36.0f, 15.0f);
	glm::vec3 rot7 = glm::vec3(-2.73f, 0, 62.64f);


	/*
	glm::vec3 pos2 = glm::vec3(45.0f, 1.2f, 65.3f);
	glm::vec3 rot2 = glm::vec3(35.5f, 0.0f, 2.64f);

	glm::vec3 pos3 = glm::vec3(12.34f, 38.5f, 16.2f);
	glm::vec3 rot3 = glm::vec3(34.5f, 0.0f, -49.8f);

	glm::vec3 pos4 = glm::vec3(-39.9f, 36.56f, -25.10f);
	glm::vec3 rot4 = glm::vec3(-124.6f, 0.0f, -36.6f);

	glm::vec3 pos5 = glm::vec3(-29.0f, -20.9f, -15.8f);
	glm::vec3 rot5 = glm::vec3(-118.2f, 0.0f, 34.4f);

	glm::vec3 pos6 = glm::vec3(0.8f, -20.89f, 33.25f);
	glm::vec3 rot6 = glm::vec3(2.16f, 0.0f, 40.48f);

	glm::vec3 pos7 = glm::vec3(25.637f, 14.45f, -20.65f);
	glm::vec3 rot7 = glm::vec3(127.68f, 0.0f, -9.039);

*/


	camArray = new std::vector<std::vector<float>>;

	std::vector<float> tmpVector = { pos1.x, pos1.y, pos1.z, rot1.x, rot1.y, rot1.z, 0.0f };
	camArray->push_back(tmpVector);

	tmpVector.clear(); 
	tmpVector = { pos2.x, pos2.y, pos2.z, rot2.x, rot2.y, rot2.z, 0.05f };
	camArray->push_back(tmpVector);


	tmpVector.clear();
	tmpVector = { pos3.x, pos3.y, pos3.z, rot3.x, rot3.y, rot3.z, 0.1f };
	camArray->push_back(tmpVector);


	tmpVector.clear();
	tmpVector = { pos4.x, pos4.y, pos4.z, rot4.x, rot4.y, rot4.z, 0.15f };
	camArray->push_back(tmpVector);


	tmpVector.clear();
	tmpVector = { pos5.x, pos5.y, pos5.z, rot5.x, rot5.y, rot5.z, 0.2f };
	camArray->push_back(tmpVector);


	tmpVector.clear();
	tmpVector = { pos6.x, pos6.y, pos6.z, rot6.x, rot6.y, rot6.z, 0.25f };
	camArray->push_back(tmpVector);


	tmpVector.clear();
	tmpVector = { pos7.x, pos7.y, pos7.z, rot7.x, rot7.y, rot7.z, 0.30f };
	camArray->push_back(tmpVector);








	pos = new std::vector<glm::vec3>;
	rotation = new std::vector<glm::vec3>;

	//glm::vec3 cp1 = glm::vec3(camArray[0][0][0], camArray[0][0][1], camArray[0][0][2]);
	glm::vec3 cp2 = glm::vec3(camArray[0][0][0], camArray[0][0][1], camArray[0][0][2]);
	glm::vec3 cp3 = glm::vec3(camArray[0][1][0], camArray[0][1][1], camArray[0][1][2]);
	glm::vec3 cp4 = glm::vec3(camArray[0][2][0], camArray[0][2][1], camArray[0][2][2]);
	glm::vec3 cp5 = glm::vec3(camArray[0][3][0], camArray[0][3][1], camArray[0][3][2]);
	glm::vec3 cp6 = glm::vec3(camArray[0][4][0], camArray[0][4][1], camArray[0][4][2]);
	glm::vec3 cp7 = glm::vec3(camArray[0][5][0], camArray[0][5][1], camArray[0][5][2]);
	glm::vec3 cp8 = glm::vec3(camArray[0][6][0], camArray[0][6][1], camArray[0][6][2]);



	//glm::vec3 rp1 = glm::vec3(camArray[0][0][3], camArray[0][0][4], camArray[0][0][5]);
	glm::vec3 rp2 = glm::vec3(camArray[0][0][3], camArray[0][0][4], camArray[0][0][5]);
	glm::vec3 rp3 = glm::vec3(camArray[0][1][3], camArray[0][1][4], camArray[0][1][5]);
	glm::vec3 rp4 = glm::vec3(camArray[0][2][3], camArray[0][2][4], camArray[0][2][5]);
	glm::vec3 rp5 = glm::vec3(camArray[0][3][3], camArray[0][3][4], camArray[0][3][5]);
	glm::vec3 rp6 = glm::vec3(camArray[0][4][3], camArray[0][4][4], camArray[0][4][5]);
	glm::vec3 rp7 = glm::vec3(camArray[0][5][3], camArray[0][5][4], camArray[0][5][5]);
	glm::vec3 rp8 = glm::vec3(camArray[0][6][3], camArray[0][6][4], camArray[0][6][5]);


	//pos->push_back(cp1);
	pos->push_back(cp2);
	pos->push_back(cp3);
	pos->push_back(cp4);
	pos->push_back(cp5);
	pos->push_back(cp6);
	pos->push_back(cp7);
	pos->push_back(cp8);

	//rotation->push_back(rp1);
	rotation->push_back(rp2);
	rotation->push_back(rp3);
	rotation->push_back(rp4);
	rotation->push_back(rp5);
	rotation->push_back(rp6);
	rotation->push_back(rp7);
	rotation->push_back(rp8);

}

void MovementCamera::startAutoCamera() {


	std::vector<float> v;
	
	for (unsigned i = 0; i < camArray->size(); i++) {
		v.push_back(camArray[0][i][6]);
	}





	std::vector<float>::iterator low, up;
	low = std::upper_bound(v.begin(),v.end(), global::t);

	int actPos = (low - v.begin());

	//std::cout << "Begin: " << (low-v.begin()) << std::endl;



	float localT = (((global::t-camArray[0][actPos-1][6]) / (camArray[0][actPos][6] - camArray[0][actPos-1][6])) + (actPos-1));

	glm::vec3 cubicsp = cubic_spline(*pos, localT);

	/*std::cout << "Position: " << cubicsp.x << " " << cubicsp.y << " " << cubicsp.z << " T: " << global::t << std::endl;
	std::cout << std::endl;
	*/
	global::camera->position = cubicsp;

	glm::vec3 cubicrp = cubic_spline(*rotation, localT);

	//std::cout << "Position: " << cubicrp.x << " " << cubicrp.y << " " << cubicrp.z << " T: " << global::t << std::endl;
	//std::cout << "NEED: " << cubicrp.x << " " << global::camera->getYaw() << std::endl;
	//std::cout << std::endl;

	//std::cout << "NEW YAW: " << cubicrp.x << " YAW: " << global::camera->getYaw() << " CALCULATED: " << (abs(cubicrp.x) - abs(global::camera->getYaw())) << std::endl;
	
	float calcYAW = 0;
	float calcPITCH = 0;

	



	//calculate YAW (probleme here are the sign) 
	if (cubicrp.x < 0) {
		if (tmpYaw < 0) {
			if (cubicrp.x < tmpYaw) {
				calcYAW = abs(abs(cubicrp.x) - abs(tmpYaw));
			}
			else {

				calcYAW = -abs(abs(cubicrp.x) - abs(tmpYaw));
			}
		}
		else {
			calcYAW = (abs(tmpYaw) + abs(cubicrp.x));
		}
	}
	else {
		if (tmpYaw < 0) {
			calcYAW = -(abs(tmpYaw) + abs(cubicrp.x));
		}
		else {
			if (tmpYaw > cubicrp.x) {
				calcYAW = abs(tmpYaw - cubicrp.x);
			}
			else {
				calcYAW = -abs(cubicrp.x - tmpYaw);
			}
		}
	}


	//calculate PITCH (probleme here are the sign) 
	if (cubicrp.z < 0) {
		if (tmpPitch < 0) {
			if (cubicrp.z < tmpPitch) {
				calcPITCH = -abs(abs(cubicrp.z) - abs(tmpPitch));
			}
			else {

				calcPITCH = abs(abs(cubicrp.z) - abs(tmpPitch));
			}
		}
		else {
			calcPITCH = -(abs(tmpPitch) + abs(cubicrp.z));
		}
	}
	else {
		if (tmpPitch < 0) {
			calcPITCH = (abs(tmpPitch) + abs(cubicrp.z));
		}
		else {
			if (tmpPitch > cubicrp.z) {
				calcPITCH = tmpPitch - cubicrp.z;
			}
			else {
				calcPITCH = cubicrp.z - tmpPitch;
			}
		}
	}



	global::camera->calcNewRotation(calcYAW, calcPITCH);
	tmpYaw = cubicrp.x;
	tmpPitch = cubicrp.z;


	//global::camera->processMouseMovement(0.5f, 0);

	//global::camera->yawAutoChange(cubicrp.x);
	//global::camera->pitchAutoChange(cubicrp.z);


	/*
	if (actPos == 1) {

		std::vector<glm::vec3>* pos = new std::vector<glm::vec3>;

		glm::vec3 cp1 = glm::vec3(camArray[0][actPos-1][0], camArray[0][actPos-1][1], camArray[0][actPos-1][2]);
		glm::vec3 cp2 = glm::vec3(camArray[0][actPos-1][0], camArray[0][actPos - 1][1], camArray[0][actPos - 1][2]);
		glm::vec3 cp3 = glm::vec3(camArray[0][actPos][0], camArray[0][actPos][1], camArray[0][actPos][2]);
		glm::vec3 cp4 = glm::vec3(camArray[0][actPos+1][0], camArray[0][actPos+1][1], camArray[0][actPos+1][2]);


		std::cout << "Position1: " << camArray[0][actPos - 1][0] << " " << camArray[0][actPos - 1][1] << " " << camArray[0][actPos - 1][2] << std::endl;
		std::cout << "Position2: " << camArray[0][actPos][0] << " " << camArray[0][actPos][1] << " " << camArray[0][actPos][2] << std::endl;
		//std::cout << std::endl;

		pos->push_back(cp1);
		pos->push_back(cp2);
		pos->push_back(cp3);
		pos->push_back(cp4);


		//float localT = global::t / (camArray[0][actPos][6] - camArray[0][actPos - 1][6]);

		glm::vec3 cubicsp = cubic_spline(*pos, global::t);

		std::cout << "Position: " << cubicsp.x << " " << cubicsp.y << " " << cubicsp.z<< " T: "<< global::t << std::endl;
		std::cout << std::endl;
		global::camera->position = cubicsp;
	}
	else {
		std::vector<glm::vec3>* pos = new std::vector<glm::vec3>;
		glm::vec3 cp1 = glm::vec3(camArray[0][actPos - 2][0], camArray[0][actPos - 2][1], camArray[0][actPos - 2][2]);
		glm::vec3 cp2 = glm::vec3(camArray[0][actPos - 1][0], camArray[0][actPos - 1][1], camArray[0][actPos - 1][2]);
		glm::vec3 cp3 = glm::vec3(camArray[0][actPos][0], camArray[0][actPos][1], camArray[0][actPos][2]);
		glm::vec3 cp4 = glm::vec3(camArray[0][actPos + 1][0], camArray[0][actPos + 1][1], camArray[0][actPos + 1][2]);
		pos->push_back(cp1);
		pos->push_back(cp2);
		pos->push_back(cp3);
		pos->push_back(cp4);
		global::camera->position = cubic_spline(*pos, (global::t));
	}
	*/

	//glm::vec3 posCam = cubic_spline(*pos, (global::t));

//	std::lower_bound(camArray->begin(), camArray->end(), 1);

	
	/*glm::vec3 posCam
	glm::vec3 posCam = cubic_spline(*pos, (global::t));


	global::camera->position = posCam;
	global::camera->
	

	std::cout << "start auto camera " << global::t << std::endl;*/
}
/*
glm::vec3 MovementCamera::cubic_spline(const std::vector<glm::vec3>& cp, float t)
{
	// indices of the relevant control points
	int i0 = glm::clamp<int>(t, 0, cp.size() - 1);
	int i1 = glm::clamp<int>(t, 0, cp.size() - 1);
	int i2 = glm::clamp<int>(t +1, 0, cp.size() - 1);
	int i3 = glm::clamp<int>(t +1, 0, cp.size() - 1);
	
	// parameter on the local curve interval
	float local_t = glm::fract(t);

	std::cout << "LOCAL T: " << local_t << std::endl;
	
	return glm::cubic(cp[0], cp[1], cp[2], cp[3], local_t);

	
}*/

glm::vec3 MovementCamera::cubic_spline(const std::vector<glm::vec3>& cp, float t)
{
	// indices of the relevant control points
	int i0 = glm::clamp<int>(t - 1, 0, cp.size() - 1);
	int i1 = glm::clamp<int>(t, 0, cp.size() - 1);
	int i2 = glm::clamp<int>(t + 1, 0, cp.size() - 1);
	int i3 = glm::clamp<int>(t + 2, 0, cp.size() - 1);

	// parameter on the local curve interval
	float local_t = glm::fract(t);

	return glm::catmullRom(cp[i0], cp[i1], cp[i2], cp[i3], local_t);
}

