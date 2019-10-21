#include "Bricks.h"

extern Breakout breakout;

Bricks::Bricks()
{
	brickModel = std::make_unique<Model>("../assets/Brick/brick.obj");

	glGenBuffers(1, &bricksIBO);
	glBindBuffer(GL_ARRAY_BUFFER, bricksIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * breakout.bricks.size(), breakout.bricks.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(brickModel->meshes[0].VAO);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, bricksIBO);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

Bricks::~Bricks()
{

}

void Bricks::drawInstanced(Shader shader) {
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < brickModel->meshes[0].textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = brickModel->meshes[0].textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, brickModel->meshes[0].textures[i].id);
	}

	glBindVertexArray(brickModel->meshes[0].VAO);
	glDrawElementsInstanced(GL_TRIANGLES, brickModel->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, breakout.bricks.size());
	glBindVertexArray(0);
}