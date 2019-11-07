#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"
#include "DirectionalLight.h"

class PhongShader
{
public:
	PhongShader();
	~PhongShader();

	GLuint FBO;
	GLuint color;
	GLuint depthmap;

	void use(glm::mat4 view, glm::mat4 projection, GLuint depthmap);
	void draw(Model model);
private:
	std::unique_ptr<Shader> shader;
	std::unique_ptr<DirectionalLight> directionalLight;
};

