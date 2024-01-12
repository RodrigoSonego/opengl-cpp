#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include <vector>

class Game
{
public:
	Game(GameObject player, SpriteRenderer renderer, Texture background);
	
	virtual void Init();
	virtual void ProcessInput(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void Draw();

	std::vector<GameObject*> objects;

private:
	SpriteRenderer m_Renderer;
	GameObject& m_Player;

	Texture m_Background;
};

