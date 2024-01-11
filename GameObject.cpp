#include "GameObject.h"

GameObject::GameObject(SubTexture& tex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color, glm::vec2 velocity)
	: Sprite(tex), Position(position) , Size(size), Rotation(rotation), Color(color), Velocity(velocity)
{
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.RenderSprite(Sprite, Position, Size, Rotation);
}

void GameObject::Draw(SpriteRenderer& renderer, int numberOfFrames)
{

}
