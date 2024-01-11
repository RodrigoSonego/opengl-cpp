#include "GameObject.h"

GameObject::GameObject(Texture& tex, glm::vec2 position, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
	: Sprite(tex), Position(position) , Size(size), Color(color), Velocity(velocity)
{
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.RenderSprite(Sprite, Position, Size, 0);
}

