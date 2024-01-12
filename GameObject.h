#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "SubTexture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	glm::vec3 Position;
	glm::vec2 Size;
	glm::vec2 Velocity;
	glm::vec3 Color;

	float Rotation;

	SubTexture Sprite;

	GameObject* parent = nullptr;

	glm::mat4 modelMatrix;

	GameObject(SubTexture& sprite, glm::vec3 position, glm::vec2 size, float rotation = 0.f, glm::vec3 color = {1.f, 1.f, 1.f}, glm::vec2 velocity = { 0.0f, 0.0f } );

	void Draw(SpriteRenderer& renderer);
	void Draw(SpriteRenderer& renderer, int numberOfFrames);
	virtual void UpdateModelMatrix();

	void printModelMatrix();

private:

};

