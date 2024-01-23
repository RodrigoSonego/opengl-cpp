#include "Game.h"
#include "Texture.h"
#include <SDL.h>
#include <iostream>

Game::Game(Player* player, SpriteRenderer renderer, TextRenderer textRenderer, Texture background, Camera camera)
	: m_Player(player), m_Renderer(renderer), m_textRenderer(textRenderer), m_Background(background), m_Camera(camera)
{
	scrollPivot.position = glm::vec3(0.0f);
}

void Game::Init()
{
	//objects[0]->parent = &scrollPivot;
	for (GameObject* obj : objects) {
		obj->parent = &scrollPivot;
	}
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
	scrollPivot.position.x -= 50 * deltaTime;

	for (GameObject* obj : objects)
	{
		obj->UpdateModelMatrix();
	}
}

void Game::Draw(float deltaTime)
{
	DrawText();

	m_Player->DrawPlayer(m_Renderer, deltaTime);
	//objects[1]->Draw(m_Renderer, 7, deltaTime);
	for (GameObject* obj : objects)
	{
		obj->Draw(m_Renderer, 20, deltaTime);
	}

	m_Renderer.RenderSprite(&m_Background, glm::vec3(0.0f, 0.0f, -0.2f), glm::vec2(800.0f, 600.f), 0.0f);
	
}

void Game::DrawText()
{
	m_textRenderer.renderText("Player one", glm::vec2(10.0f, 10.0f), 0.8f, glm::vec3(1.f, 1.f, 1.f));
	m_textRenderer.renderText("0004206900", glm::vec2(10.0f, 26.0f), 1.4f, glm::vec3(1.f, 1.f, 1.f));
	m_textRenderer.renderText("High Score", glm::vec2(340.f, 10.0f), 0.8f, glm::vec3(1.f, 1.f, 1.f));
	m_textRenderer.renderText("9999999998", glm::vec2(330.f, 26.0f), 0.9f, glm::vec3(1.f, 1.f, 1.f));
}
