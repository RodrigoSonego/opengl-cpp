#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "Camera.h"
#include "Player.h"
#include <vector>
#include "TextRenderer.h"


#define SCROLL_SPEED 50
#define PARALAX_SPEED 15
class Game
{
public:
	Game(Player* player, SpriteRenderer renderer, TextRenderer textRenderer, Texture background, Camera camera);
	
	virtual void Init();
	virtual void ProcessInput(float deltaTime, SDL_Event event);
	virtual void Update(float deltaTime);
	virtual void Draw(float deltaTime);

	std::vector<GameObject*> Objects;

	std::vector<GameObject*> ParalaxObjects;
private:
	SpriteRenderer m_Renderer;
	TextRenderer m_textRenderer;
	Player* m_Player;
	Camera& m_Camera;

	Transform m_ScrollPivot;
	Transform m_ParalaxPivot;

	Texture m_Background;

	void DrawText();
};

