#include "VoidShader.h"

VoidShader::VoidShader()
{
	voidModel = std::make_unique<Model>("../assets/Void/void.obj");
	voidShader = std::make_unique<Shader>("../shader/void.vert", "../shader/void.frag");

	voidModel->model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -25.0f, 0.0f));
}

VoidShader::~VoidShader()
{

}

void VoidShader::draw(glm::mat4 view, glm::mat4 projection)
{
	voidShader->use();
	voidShader->setMat4("model", voidModel->model);
	voidShader->setMat4("projection", projection);
	voidShader->setMat4("view", view);
	voidShader->setVec2("screenSize", glm::vec2(setting::SCREEN_WIDTH, setting::SCREEN_HEIGHT));

	Mesh mesh = voidModel->meshes[0];

	glActiveTexture(GL_TEXTURE0);
	voidShader->setInt("aTexture", 0);
	glBindTexture(GL_TEXTURE_2D, mesh.textures[0].id);

	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}