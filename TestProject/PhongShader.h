#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "Model.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "FrameBuffer.h"

class PhongShader
{
public:
	PhongShader();
	~PhongShader();

	std::unique_ptr<FrameBuffer> framebuffer;

	void use(glm::mat4 view, glm::mat4 projection, GLuint depthmap);
	void draw(Model model, bool light = false);
	void drawInstanced(Model model, int size);
private:
	std::unique_ptr<Shader> shader;
	std::unique_ptr<DirectionalLight> directionalLight;
};

