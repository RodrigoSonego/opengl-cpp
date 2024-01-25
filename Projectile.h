#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(SubTexture& sprite, glm::vec3 position, glm::vec2 size, float duration, SubTexture& explosion, float rotation = 0.f, glm::vec3 color = { 1.f, 1.f, 1.f }, glm::vec2 velocity = { 0.0f, 0.0f });

	void DrawAnimating(SpriteRenderer& renderer, int numberOfFrames, float deltaTime, bool willLoop = true) override;
	void Shoot(glm::vec2 startPosition, glm::vec2 offset);

private:
	enum ProjectileState {
		Moving,
		Exploding,
		Inactive
	};

	void Reset();
	void Explode();

	ProjectileState m_State;

	SubTexture* originalSprite;
	SubTexture* m_ExplosionSprite;

	float m_duration;
	float m_elapsedTime = 0.0f;
};

