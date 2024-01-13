#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Camera.h"
#include <vector>

class Game
{
public:
	Game(GameObject player, SpriteRenderer renderer, Texture background, Camera camera);
	
	virtual void Init();
	virtual void ProcessInput(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void Draw();

	std::vector<GameObject*> objects;

private:
	SpriteRenderer m_Renderer;
	GameObject& m_Player;
	Camera& m_Camera;

	Transform scrollPivot;

	Texture m_Background;
};

