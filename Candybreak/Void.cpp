#include "Void.h"

extern glm::vec2 screenSize;
extern const float BOX_SIZE;

Void::Void()
{
	voidModel = std::make_unique<Model>("../assets/Void/void.obj");
	voidShader = std::make_unique<Shader>("../shader/void.vert", "../shader/void.frag");
}

Void::~Void()
{

}

void Void::draw(glm::mat4 view, glm::mat4 projection)
{
	voidShader->use();
	voidShader->setMat4("projection", projection);
	voidShader->setMat4("view", view);
	voidShader->setVec2("screenSize", screenSize);

	voidModel->Position = glm::vec3(0.0f, -BOX_SIZE / 2.0f, 0.0f);

	voidModel->Draw(*voidShader);
}