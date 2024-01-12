#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GameObject::GameObject(SubTexture& tex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec3 color, glm::vec2 velocity)
	: Sprite(tex), Position(position) , Size(size), Rotation(rotation), Color(color), Velocity(velocity)
{
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	//renderer.RenderSprite(Sprite, Position, Size, Rotation);

    std::cout << "drawing sprite at position " << Position.x << ", " << Position.y << std::endl;
    renderer.RenderSprite(Sprite, modelMatrix, Color);
}

void GameObject::Draw(SpriteRenderer& renderer, int numberOfFrames)
{

}

void GameObject::UpdateModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(Position));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * Size.x, 0.5f * Size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * Size.x, -0.5f * Size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(Size, 1.0f)); // last scale

    if (parent != nullptr) {

        glm::mat4 parentModel = glm::mat4(1.0f);
        parentModel = glm::translate(parentModel, parent->Position);

        model = parentModel * model;
    }

    modelMatrix = model;
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

