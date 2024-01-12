#include "Game.h"
#include "Texture.h"
#include <SDL.h>
#include <iostream>

Game::Game(GameObject player, SpriteRenderer renderer, Texture background)
	: m_Player(player), m_Renderer(renderer), m_Background(background)
{
	objects.push_back(&player);
}

void Game::Init()
{
	std::cout << "size dos obj: " << objects.size() << std::endl;

	objects[1]->parent = &m_Player;
}

void Game::ProcessInput(float deltaTime)
{
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_W]) {
		m_Player.Position.y -= m_Player.Velocity.y * deltaTime;
	}
	else if (keyState[SDL_SCANCODE_S])
		m_Player.Position.y += m_Player.Velocity.y * deltaTime;
}

void Game::Update(float deltaTime)
{
	//m_Player.UpdateModelMatrix();
	//objects[0]->UpdateModelMatrix();
	for (GameObject* obj : objects)
	{
		obj->UpdateModelMatrix();
	}
}

void Game::Draw()
{
	//m_Player.Draw(m_Renderer);
	//objects[0]->Draw(m_Renderer);
	for (GameObject* obj : objects)
	{
		obj->Draw(m_Renderer);
	}

	//m_Renderer.RenderSprite(m_Background, glm::vec3(0.0f, 0.0f, -0.2f), glm::vec2(800.0f, 600.f), 0.0f);
	
}
