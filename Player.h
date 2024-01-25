#pragma once
#include "GameObject.h"
#include "Projectile.h"

class Player : public GameObject
{
public:
	enum PlayerState
	{
		Idle,
		MovingUp,
		MovingDown,
	};

	using GameObject::GameObject;
	void DrawPlayer(SpriteRenderer& renderer, float deltaTime);
	void SetPlayerState(PlayerState state);
	void shootMissile();
	void setMissile(Projectile* missile);

private:
	PlayerState m_State = PlayerState::Idle;

	int m_FramesToAnim = 0;
	Projectile* m_Missile;
};

