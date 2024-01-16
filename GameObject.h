#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "SubTexture.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "Animator.h"

class GameObject
{
public:
	glm::vec2 Velocity;
	glm::vec3 Color;

	Transform transform;

	SubTexture* Sprite;

	Transform* parent = nullptr;


	explicit GameObject(SubTexture& sprite, glm::vec3 position, glm::vec2 size, float rotation = 0.f, glm::vec3 color = {1.f, 1.f, 1.f}, glm::vec2 velocity = { 0.0f, 0.0f } );

	virtual void Draw(SpriteRenderer& renderer);
	virtual void Draw(SpriteRenderer& renderer, int numberOfFrames, float deltaTime);
	virtual void UpdateModelMatrix();

	void printModelMatrix();

protected:
	Animator animator;

private:
	glm::mat4 modelMatrix;

};

