#include "Game.h"
#include <SDL.h>

Game::Game(GameObject player, SpriteRenderer renderer) : m_Player(player), m_Renderer(renderer)
{
}

void Game::Init()
{
}

void Game::ProcessInput(float deltaTime)
{
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_W])
		m_Player.Position.y -= m_Player.Velocity.y * deltaTime;
	else if (keyState[SDL_SCANCODE_S])
		m_Player.Position.y += m_Player.Velocity.y * deltaTime;
}

void Game::Update(float deltaTime)
{
}

void Game::Draw()
{
	m_Player.Draw(m_Renderer);
}
