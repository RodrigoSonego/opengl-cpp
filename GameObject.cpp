#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GameObject::GameObject(SubTexture& tex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec3 color, glm::vec2 velocity)
	: Sprite(&tex), Color(color), Velocity(velocity)
{
	transform.position = position;
	transform.size = size;
	transform.rotation = rotation;
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	if (m_WillDraw == false) { return; }

	renderer.RenderSprite(*Sprite, modelMatrix, Color);
}

void GameObject::Draw(SpriteRenderer& renderer, int numberOfFrames, float deltaTime, bool willLoop)
{
	animator.play(Sprite, numberOfFrames, deltaTime, willLoop);

	Draw(renderer);
}

void GameObject::Draw(SpriteRenderer& renderer, float deltaTime, bool willLoop)
{
	int totalFrames = Sprite->getMaxNumberOfFrames();

	Draw(renderer, totalFrames, deltaTime, willLoop);
}

void GameObject::UpdateModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transform.position);  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * transform.size.x, 0.5f * transform.size.y, 0.0f)); // move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
	model = glm::translate(model, glm::vec3(-0.5f * transform.size.x, -0.5f * transform.size.y, 0.0f)); // move origin back

	model = glm::scale(model, glm::vec3(transform.size, 1.0f)); // last scale

	if (parent != nullptr) {

		glm::mat4 parentModel = glm::mat4(1.0f);
		parentModel = glm::translate(parentModel, parent->position);

		model = parentModel * model;
	}

	modelMatrix = model;
}

void GameObject::setWillDraw(bool willDraw)
{
	m_WillDraw = willDraw;
}

void GameObject::printModelMatrix()
{
	std::cout << "Model Matrix:" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << modelMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

