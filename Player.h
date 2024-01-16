#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	enum PlayerState
	{
		Idle,
		MovingUp,
		MovingDown,
		UpToIdle,
		DownToIdle
	};

	using GameObject::GameObject;
	void DrawPlayer(SpriteRenderer& renderer, float deltaTime);
	void setPlayerState(PlayerState state);

private:
	PlayerState m_State = PlayerState::Idle;

	int framesToAnim = 0;
};

