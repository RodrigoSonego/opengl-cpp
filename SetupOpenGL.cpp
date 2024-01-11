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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/// <summary>
/// Does SDL startup
/// </summary>
/// <returns></returns>
SDL_Window* startOpenGLWindow();
void captureMouse();
void stopCapturingMouse();
void processEscapePress();
void processDrawModeChange();

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

	SpriteRenderer renderer(shader);

#pragma region Texture
	// Create and bind texture afterwards
	Texture mainTexture("res/textures/graphics/Ship1.bmp", GL_RGB);

	shader.use();

	shader.setTextureIndex("image", 0);
	mainTexture.bindTexture(0);

#pragma endregion
	


	glm::mat4 transMat(1.0f);

	shader.setMat4("transform", transMat);

#pragma region Camera
	// // // // CAMERA STUFF // // // //
	
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	//glm::mat4 projection;

	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH,
		(float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

	shader.setMat4("projection", projection);

	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;


#pragma endregion

	glClearColor(0.0f, 0.6f, 0.3f, 1.0f);

	int start = SDL_GetTicks();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	glm::vec2 position = glm::vec2(40.0f, 50.0f);
	glm::vec2 size = glm::vec2(100.0f, 100.0f);
	float rotate = 90.f;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 velocity = glm::vec2(0.0f, 200.0f);

	SubTexture sprite = SubTexture::createFromIndexes(mainTexture, glm::vec2(3, 0), glm::vec2(64, 64));

	GameObject player(sprite, position, size, rotate, color, velocity);
	Game game(player, renderer);


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
		game.ProcessInput(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		game.Draw();

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

void captureMouse()
{
	SDL_ShowCursor(SDL_DISABLE);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void stopCapturingMouse()
{
	SDL_ShowCursor(SDL_ENABLE);
	SDL_CaptureMouse(SDL_FALSE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void processEscapePress()
{
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	
	if (keyState[SDL_SCANCODE_ESCAPE]) { stopCapturingMouse(); }
}

void processDrawModeChange() {
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if(keyState[SDL_SCANCODE_1])
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(keyState[SDL_SCANCODE_2])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (keyState[SDL_SCANCODE_3])
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}
