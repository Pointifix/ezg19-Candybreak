#pragma once
#include <iostream>
#include "Shader.h"
#include "Model.h"

class DepthShader
{
public:
	DepthShader();                                                                                                                          
	~DepthShader();

	GLuint FBO;
	GLuint depthmap;

	void use(glm::mat4 view, glm::mat4 projection);
	void draw(Model model);
	void finish();
private:
	std::unique_ptr<Shader> shader;
};

