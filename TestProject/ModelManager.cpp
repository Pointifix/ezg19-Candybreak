#include "ModelManager.h"
#include "Globals.h"

ModelManager::ModelManager()
{
	this->map = std::make_unique<Model>("../assets/Map/simple_candyland.obj");
	this->tree = std::make_unique<Model>("../assets/Map/tree.obj");
	this->brick = std::make_unique<Model>("../assets/Brick/brick.obj");
	this->ball = std::make_unique<Model>("../assets/Ball/ball.obj");
	this->pad = std::make_unique<Model>("../assets/Pad/pad.obj");
	this->light = std::make_unique<Model>("../assets/Light/light.obj");
	this->plane = std::make_unique<Model>("../assets/Light/plane.obj");

	this->light->model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 0.0f, 0.0f));

	bricks = std::make_unique<Bricks>(this->brick->meshes[0].VAO);

	vector<Vertex> screenQuadVertices = {
		{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};

	vector<unsigned int> screenQuadIndices = {
		0,1,2,
		0,3,1
	};

	vector<Texture> screenQuadTextures;

	this->screenQuad = std::make_unique<Mesh>(screenQuadVertices, screenQuadIndices, screenQuadTextures, nullptr);
	global::screenQuadVAO = this->screenQuad->VAO;
}


ModelManager::~ModelManager()
{
}
