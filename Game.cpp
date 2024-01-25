#include "Game.h"
#include "Texture.h"
#include <SDL.h>
#include <iostream>
#include <algorithm>

Game::Game(Player* player, SpriteRenderer renderer, TextRenderer textRenderer, Texture background, Camera camera)
	: m_Player(player), m_Renderer(renderer), m_textRenderer(textRenderer),
	m_Background(background), m_Camera(camera)
{
	m_ScrollPivot.position = glm::vec3(0.0f);
	m_ParalaxPivot.position = glm::vec3(0.0f);
}

void Game::Init()
{
	for (GameObject* obj : Objects) {
		if (Projectile* proj = dynamic_cast<Projectile*>(obj)) {
			continue;
		}

		obj->parent = &m_ScrollPivot;
	}

	for (GameObject* obj : ParalaxObjects) {
		obj->parent = &m_ParalaxPivot;
	}
}

void Game::ProcessInput(float deltaTime, SDL_Event ev)
{
	const Uint8* keyState = SDL_GetKeyboardState(NULL);


	if (keyState[SDL_SCANCODE_W]) {
		m_Player->transform.position.y -= m_Player->Velocity.y * deltaTime;
		m_Player->SetPlayerState(Player::MovingUp);
	}
	else if (keyState[SDL_SCANCODE_S]) {
		m_Player->transform.position.y += m_Player->Velocity.y * deltaTime;
		m_Player->SetPlayerState(Player::MovingDown);
	}
	else {
		m_Player->SetPlayerState(Player::Idle);
	}

	if (keyState[SDL_SCANCODE_D]) {
		m_Player->transform.position.x += m_Player->Velocity.x * deltaTime;
	}
	else if (keyState[SDL_SCANCODE_A]) {
		m_Player->transform.position.x -= m_Player->Velocity.x * deltaTime;
	}

	if (keyState[SDL_SCANCODE_SPACE]) {
		m_Player->shootMissile();
	}

	float maxY = 600 - m_Player->transform.size.y;
	float maxX = 800 - m_Player->transform.size.x;

	m_Player->transform.position.y = glm::clamp(m_Player->transform.position.y, 0.0f, maxY);
	m_Player->transform.position.x = glm::clamp(m_Player->transform.position.x, 0.0f, maxX);

}

void Game::Update(float deltaTime)
{
	m_Player->UpdateModelMatrix();
	m_ScrollPivot.position.x -= SCROLL_SPEED * deltaTime;
	m_ParalaxPivot.position.x -= PARALAX_SPEED * deltaTime;

	for (GameObject* obj : Objects)
	{
		obj->UpdateModelMatrix();
	}

	for (GameObject* obj : ParalaxObjects)
	{
		obj->UpdateModelMatrix();
	}
}

void Game::Draw(float deltaTime)
{
	DrawText();

	for (GameObject* obj : ParalaxObjects)
	{
		obj->Draw(m_Renderer);
	}

	m_Player->DrawPlayer(m_Renderer, deltaTime);

	for (GameObject* obj : Objects)
	{
		obj->DrawAnimating(m_Renderer, deltaTime);
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
