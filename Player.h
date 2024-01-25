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
	/// <summary>
	/// Draws Player based on its state
	/// </summary>
	/// <param name="renderer"></param>
	/// <param name="deltaTime"></param>
	void DrawPlayer(SpriteRenderer& renderer, float deltaTime);
	/// <summary>
	/// Sets Player state and defines number of frames to be animated
	/// </summary>
	/// <param name="state"></param>
	void SetPlayerState(PlayerState state);

	void shootMissile();
	void setMissile(Projectile* missile);

private:
	PlayerState m_State = PlayerState::Idle;

	int m_FramesToAnim = 0;
	Projectile* m_Missile;
};

