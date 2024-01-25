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
	/// <summary>
	/// Draw animating a specific number of frames
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="numberOfFrames">number of frames to be animated</param>
	/// <param name="deltaTime"></param>
	/// <param name="willLoop"></param>
	virtual void DrawAnimating(SpriteRenderer& renderer, int numberOfFrames, float deltaTime, bool willLoop = true);
	/// <summary>
	/// Draw animating all frames of the spritesheet
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="deltaTime"></param>
	/// <param name="willLoop"></param>
	virtual void DrawAnimating(SpriteRenderer& renderer, float deltaTime, bool willLoop = true);
	/// <summary>
	/// Calculates the model matrix based on its transform and that of its parent (if it has one)
	/// </summary>
	virtual void UpdateModelMatrix();

	void setWillDraw(bool willDraw);

protected:
	/// <summary>
	/// Responsible for the animation of the object, can be accessed by inherited classes
	/// </summary>
	Animator animator;

private:
	glm::mat4 modelMatrix;
	
	bool m_WillDraw = true;
};

