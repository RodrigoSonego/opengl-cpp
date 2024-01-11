#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SpriteRenderer.h"

class Game
{
public:
	Game(GameObject player, SpriteRenderer renderer);
	
	virtual void Init();
	virtual void ProcessInput(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void Draw();


private:
	SpriteRenderer m_Renderer;
	GameObject m_Player;
};

