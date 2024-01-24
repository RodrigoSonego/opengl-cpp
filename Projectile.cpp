#include "Projectile.h"
#include <iostream>

Projectile::Projectile(SubTexture& sprite, glm::vec3 position, glm::vec2 size, float duration, SubTexture& explosion, float rotation, glm::vec3 color, glm::vec2 velocity)
	: GameObject(sprite, position, size, rotation, color, velocity),
	m_duration(duration), m_ExplosionSprite(&explosion)
{
	originalSprite = Sprite;
	Reset();
}

void Projectile::Draw(SpriteRenderer& renderer, int numberOfFrames, float deltaTime, bool willLoop)
{
	if (m_State == Inactive) { return; }

	if (m_State == Exploding && animator.isFinished) {
		Reset();
		return;
	}

	m_elapsedTime += deltaTime;
	if (m_elapsedTime >= m_duration) {
		Explode();
		GameObject::Draw(renderer, 10, deltaTime, false);
		return;
	}
	transform.position.x += Velocity.x * deltaTime;
	GameObject::Draw(renderer, 2, deltaTime);
}

void Projectile::Shoot(glm::vec2 startPosition, glm::vec2 offset)
{
	transform.position = glm::vec3(startPosition + offset, 0.0f);
	setWillDraw(true);
	m_State = Moving;
}

void Projectile::UpdateModelMatrix()
{
	if (m_State == Moving) {
;
	}

	GameObject::UpdateModelMatrix();
}

void Projectile::Reset()
{
	setWillDraw(false);
	m_elapsedTime = 0.0f;
	m_State = Inactive;
}

void Projectile::Explode()
{
	Sprite = m_ExplosionSprite;
	m_State = Exploding;
}
