#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "SubTexture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	glm::vec2 Position;
	glm::vec2 Size;
	glm::vec2 Velocity;
	glm::vec3 Color;

	float Rotation;

	SubTexture Sprite;

	GameObject(SubTexture& sprite, glm::vec2 position, glm::vec2 size, float rotation = 0.f, glm::vec3 color = {1.f, 1.f, 1.f}, glm::vec2 velocity = { 0.0f, 0.0f } );

	void Draw(SpriteRenderer& renderer);
	void Draw(SpriteRenderer& renderer, int numberOfFrames);
};

