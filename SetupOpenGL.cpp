#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "dependencies/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BufferObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>
#include "FileReader.h"

#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Game.h"
#include "TextRenderer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/// <summary>
/// Does SDL startup
/// </summary>
/// <returns></returns>
SDL_Window* startOpenGLWindow();
Player& generatePlayer(SubTexture& subTex);

int main(int argc, char** argv)
{

#pragma region Start Window

	SDL_Window* window = startOpenGLWindow();
	if (window == nullptr)
	{
		std::cout << "Failed to create SDL Window" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

#pragma endregion

	Shader shader("vertex.vert", "fragment.frag");
	Shader textShader("textVert.vert", "fragment.frag");

#pragma region Texture
	// Create and bind texture afterwards
	Texture mainTexture("res/textures/graphics/Ship1.bmp", GL_RGB);
	Texture background("res/textures/graphics/galaxy2.bmp", GL_RGB);
	Texture fontTexture("res/textures/graphics/font16x16.bmp", GL_RGB);

	shader.use();

	shader.setTextureIndex("image", 0);
	mainTexture.bindTexture(0);

#pragma endregion
	


	glm::mat4 transMat(1.0f);

	shader.setMat4("transform", transMat);

#pragma region Camera
	// // // // CAMERA STUFF // // // //
	
	Camera camera(glm::vec3(0.0f, 0.0f, 0.5f));

	//glm::mat4 projection;

	glm::mat4 projection = glm::ortho(0.0f, 800.f, 600.f, 0.0f);

	shader.setMat4("projection", projection);

	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

	SpriteRenderer renderer(shader);

#pragma endregion

	glClearColor(0.0f, 0.6f, 0.3f, 1.0f);

	int start = SDL_GetTicks();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	SubTexture sprite = SubTexture::createFromIndexes(&mainTexture, glm::vec2(3, 0), glm::vec2(64, 64));

	Player player = generatePlayer(sprite);

	Texture asteroidTex("res/textures/graphics/GAster64.bmp", GL_RGB);
	SubTexture astSprite = SubTexture::createFromIndexes(&asteroidTex, glm::vec2(7.f, 0.f), glm::vec2(64.f, 64.f));
	GameObject ast(astSprite, glm::vec3(100, 100, 0), glm::vec2(100, 100));

	TextRenderer textRenderer(&textShader, &fontTexture, glm::vec2(16.0f, 16.0f));
	Game game(&player, renderer, textRenderer, background, camera);
	game.objects.push_back(&ast);
	//game.objects.push_back(player);

	game.Init();

	SDL_Event windowEvent;
	while (true)
	{
		int now = SDL_GetTicks();

		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;

		// Event loop
		while (SDL_PollEvent(&windowEvent) != 0) {
			if (windowEvent.type == SDL_QUIT) break;
			
		}
		game.ProcessInput(deltaTime, windowEvent);

		game.Update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.setMat4("view", camera.getView());
		
		
		game.Draw(deltaTime);

		SDL_GL_SwapWindow(window);

	}

	
	return 0;
}

SDL_Window* startOpenGLWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	return window;
}

Player& generatePlayer(SubTexture& subTex)
{
	glm::vec3 position = glm::vec3(20.0f, 300.0f, 0.0f);
	glm::vec2 size = glm::vec2(100.0f, 100.0f);
	float rotate = 90.f;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 velocity = glm::vec2(200.0f, 200.0f);

	Player player(subTex, position, size, rotate, color, velocity);
	return player;
}
