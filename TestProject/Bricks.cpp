#include "Bricks.h"

Bricks::Bricks(GLuint VAO)
{
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);

	breakout::bricksPositionMutex.lock();
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * breakout::bricksPosition.size(), breakout::bricksPosition.data(), GL_STATIC_DRAW);
	breakout::bricksPositionMutex.unlock();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

Bricks::~Bricks()
{
}

void Bricks::update()
{
	if (breakout::updateBricks)
	{
		breakout::updateBricks = false;

		breakout::bricksToRemoveMutex.lock();
		breakout::bricksPositionMutex.lock();
		for (auto const& brickToRemove : breakout::bricksToRemove) {
			breakout::bricksPosition.erase(std::remove(breakout::bricksPosition.begin(), breakout::bricksPosition.end(), brickToRemove), breakout::bricksPosition.end());
		}
		breakout::bricksToRemove.clear();
		breakout::bricksPositionMutex.unlock();
		breakout::bricksToRemoveMutex.unlock();

		glBindBuffer(GL_ARRAY_BUFFER, IBO);
		breakout::bricksPositionMutex.lock();
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * breakout::bricksPosition.size(), breakout::bricksPosition.data(), GL_STATIC_DRAW);
		breakout::bricksPositionMutex.unlock();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
