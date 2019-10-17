#pragma once
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Mesh.h"
#include "Shader.h"

using namespace std;

class Model
{
public:
	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;

	glm::vec3 Position, RotationAxes;
	float Rotation;

	Model(string const &path, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotationAxes = glm::vec3(0.0f, 1.0f, 0.0f), float rotation = 0.0f);
	~Model();

	void Draw(Shader shader);

private:
	void loadModel(string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

