#include "Player.h"
#include "iostream"

void Player::DrawPlayer(SpriteRenderer& renderer, float deltaTime)
{
	switch (m_State)
	{
	case Player::MovingUp:
		animator.play(Sprite, framesToAnim, deltaTime, false, true, "up");
		break;
	case Player::MovingDown:
		animator.play(Sprite, framesToAnim, deltaTime, false, false, "down");
		break;
	case Player::Idle:
		bool isBackwards = framesToAnim > 0;

		animator.play(Sprite, abs(framesToAnim), deltaTime, false, isBackwards, "idle");
		break;
	}

	Draw(renderer);
}

void Player::SetPlayerState(PlayerState state)
{
	// Get the frame coount to be used on an animation
	// This is sorta harcoded for this specific project, and could count as a gambiarra
	// Only gets the frame count when the state is different than the current one
	// Had to do this way since it is called every frame
	if (state == PlayerState::Idle && m_State != PlayerState::Idle) {
		framesToAnim = Sprite->getSpritePosition().x - 3;
	}
	else if (state == PlayerState::MovingUp && m_State != PlayerState::MovingUp) {
		framesToAnim = Sprite->getSpritePosition().x;
	}
	else if (state == PlayerState::MovingDown && m_State != PlayerState::MovingDown) {
		framesToAnim = 6 - Sprite->getSpritePosition().x;
	}
	
	//std::cout << "spritePos: " << Sprite->getSpritePosition().x << " framesToAnim: " << abs(framesToAnim) << std::endl;
	m_State = state;
}

void Player::shootMissile()
{
	glm::vec2 offset(transform.size.x, (transform.size.y / 2) - (m_Missile->transform.size.y/2));
	m_Missile->Shoot(transform.position, offset);
}

void Player::setMissile(Projectile* missile)
{
	m_Missile = missile;
}
