#pragma once
#include "stb_image.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Globals.h"

GLuint skyboxVAO;
GLuint skyboxVBO;

GLuint texture1ID;
GLuint texture2ID;

std::unique_ptr<Shader> skyboxShader;

float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

void initSkybox()
{
	skyboxShader = std::make_unique<Shader>("../shader/skybox.vert", "../shader/skybox.frag");

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::string skybox1 = "Bluesky/bluesky";

	vector<std::string> faces1
	{
		"../assets/Skybox/" + skybox1 + "_right1.png",
		"../assets/Skybox/" + skybox1 + "_left2.png",
		"../assets/Skybox/" + skybox1 + "_top3.png",
		"../assets/Skybox/" + skybox1 + "_bottom4.png",
		"../assets/Skybox/" + skybox1 + "_front5.png",
		"../assets/Skybox/" + skybox1 + "_back6.png"
	};

	std::string skybox2 = "Nebula/nebula";

	vector<std::string> faces2
	{
		"../assets/Skybox/" + skybox2 + "_right1.png",
		"../assets/Skybox/" + skybox2 + "_left2.png",
		"../assets/Skybox/" + skybox2 + "_top3.png",
		"../assets/Skybox/" + skybox2 + "_bottom4.png",
		"../assets/Skybox/" + skybox2 + "_front5.png",
		"../assets/Skybox/" + skybox2 + "_back6.png"
	};

	glGenTextures(1, &texture1ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1ID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces1.size(); i++)
	{
		unsigned char *data = stbi_load(faces1[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Skybox texture failed to load: " << faces1[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &texture2ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2ID);

	for (unsigned int i = 0; i < faces2.size(); i++)
	{
		unsigned char *data = stbi_load(faces2[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Skybox texture failed to load: " << faces2[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	skyboxShader->use();
	skyboxShader->setInt("skybox1", glGetUniformLocation(skyboxShader->ID, "skybox1"));
	skyboxShader->setInt("skybox2", glGetUniformLocation(skyboxShader->ID, "skybox2"));
}

void drawSkybox(glm::mat4 view, glm::mat4 projection)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader->use();
	glm::mat4 viewModified = glm::mat4(glm::mat3(glm::rotate(view, glm::radians((float)global::t * 90.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
	skyboxShader->setMat4("view", viewModified);
	skyboxShader->setMat4("projection", projection);
	skyboxShader->setFloat("t", (float)global::t);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture1ID);
	glUniform1i(glGetUniformLocation(skyboxShader->ID, "skybox1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture2ID);
	glUniform1i(glGetUniformLocation(skyboxShader->ID, "skybox2"), 1);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}