#pragma once
#include "Shader.h"
#include "Model.h"

const float OFF_SCREEN_RENDER_RATIO = 1.0f;

class GodRays
{
public:
	GodRays();
	~GodRays();

	void init();

	void draw(Model map, glm::mat4 view, glm::mat4 projection);
private:
	std::unique_ptr<Shader> godRaysShader;
	std::unique_ptr<Model>quad, ball;

	GLuint VBO, VAO, EBO;

	GLuint fboTextureId;
	GLuint fboId;
	GLuint rboId;
};

