#include "Game.h"
#include "Texture.h"
#include <SDL.h>
#include <iostream>

Game::Game(Player* player, SpriteRenderer renderer, Texture background, Camera camera)
	: m_Player(player), m_Renderer(renderer), m_Background(background), m_Camera(camera)
{
	scrollPivot.position = glm::vec3(0.0f);
}

void Game::Init()
{
	//objects[0]->parent = &scrollPivot;
}

void Game::ProcessInput(float deltaTime, SDL_Event ev)
{
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_W]) {
		m_Player->transform.position.y -= m_Player->Velocity.y * deltaTime;
		m_Player->setPlayerState(Player::MovingUp);
	}
	else if (keyState[SDL_SCANCODE_S]) {
		m_Player->transform.position.y += m_Player->Velocity.y * deltaTime;
		m_Player->setPlayerState(Player::MovingDown);
	}
	else {
		m_Player->setPlayerState(Player::Idle);
	}

	float maxY = 600 - m_Player->transform.size.y;
	
	if (m_Player->transform.position.y > maxY) {
		m_Player->transform.position.y = maxY;
		return;
	}
	
	if (m_Player->transform.position.y < 0) {
		m_Player->transform.position.y = 0;
	}

}

void Game::Update(float deltaTime)
{
	m_Player->UpdateModelMatrix();
	//scrollPivot.position.x -= 30 * deltaTime;

	/*for (GameObject* obj : objects)
	{
		obj->UpdateModelMatrix();
	}*/
}

void Game::Draw(float deltaTime)
{
	m_Player->DrawPlayer(m_Renderer, deltaTime);
	//objects[1]->Draw(m_Renderer, 7, deltaTime);
	/*for (GameObject* obj : objects)
	{
		obj->Draw(m_Renderer);
	}*/

	//m_Renderer.RenderSprite(&m_Background, glm::vec3(0.0f, 0.0f, -0.2f), glm::vec2(800.0f, 600.f), 0.0f);
	
}
