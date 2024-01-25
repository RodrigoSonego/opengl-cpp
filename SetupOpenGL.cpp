#include <iostream>
#include <glad/glad.h>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BufferObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>
#include "FileReader.h"

#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Game.h"
#include "Projectile.h"
#include "TextRenderer.h"

#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/// <summary>
/// Does SDL startup
/// </summary>
/// <returns></returns>
SDL_Window* startOpenGLWindow();
Player& generatePlayer(SubTexture& subTex);
glm::vec2 getRandomPos();

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

	Shader spriteShader("vertex.vert", "fragment.frag");
	Shader textShader("textVert.vert", "fragment.frag");

#pragma region Texture
	// Create and bind texture afterwards
	Texture shipTexture("res/textures/graphics/Ship1.bmp", GL_RGB);
	Texture background("res/textures/graphics/galaxy2.bmp", GL_RGB);
	Texture fontTexture("res/textures/graphics/font16x16.bmp", GL_RGB);
	Texture sAsteroidTex("res/textures/graphics/SAster64.bmp", GL_RGB);
	Texture mAsteroidTex("res/textures/graphics/MAster64.bmp", GL_RGB);
	Texture lonerTex("res/textures/graphics/LonerA.bmp", GL_RGB);
	Texture rusherTex("res/textures/graphics/rusher.bmp", GL_RGB);
	Texture droneTex("res/textures/graphics/drone.bmp", GL_RGB);
	Texture shieldTex("res/textures/graphics/PUShield.bmp", GL_RGB);
	Texture weaponTex("res/textures/graphics/PUWeapon.bmp", GL_RGB);
	Texture missileTex("res/textures/graphics/missile.bmp", GL_RGB);
	Texture explosionTex("res/textures/graphics/explode16.bmp", GL_RGB);

	std::vector<GameObject*> gameObjects;

	spriteShader.use();
	spriteShader.setTextureIndex("image", 0);

#pragma endregion

#pragma region ObjectsSubTextures
	SubTexture sAstSprite = SubTexture::createFromIndexes(&sAsteroidTex, glm::vec2(0.f), glm::vec2(64.f));
	SubTexture mAstSprite = SubTexture::createFromIndexes(&mAsteroidTex, glm::vec2(0.f), glm::vec2(64.f));
	SubTexture lonerSprite = SubTexture::createFromIndexes(&lonerTex, glm::vec2(0.f), glm::vec2(64.f));
	SubTexture rusherSprite = SubTexture::createFromIndexes(&rusherTex, glm::vec2(0.f), glm::vec2(64.f, 32.f));
	SubTexture droneSprite = SubTexture::createFromIndexes(&droneTex, glm::vec2(0.f), glm::vec2(32.f));
	SubTexture shieldSprite = SubTexture::createFromIndexes(&shieldTex, glm::vec2(0.f), glm::vec2(32.f));
	SubTexture weaponSprite = SubTexture::createFromIndexes(&weaponTex, glm::vec2(0.f), glm::vec2(32.f));
	SubTexture sMissileSprite = SubTexture::createFromIndexes(&missileTex, glm::vec2(0.f), glm::vec2(16.f));
	SubTexture explosionSprite = SubTexture::createFromIndexes(&explosionTex, glm::vec2(0.f), glm::vec2(16.f));

#pragma endregion

	srand(static_cast <unsigned> (time(0)));

#pragma region GameObject creation

	GameObject sAsteroidObj(sAstSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80));
	GameObject sAsteroid1Obj(sAstSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80));
	GameObject sAsteroid2Obj(sAstSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80));
	GameObject mAsteroidObj(mAstSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80));
	GameObject mAsteroid1Obj(mAstSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80));
	GameObject lonerObj(lonerSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80));
	GameObject rusherObj(rusherSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80.0f, 40.0f));
	GameObject droneObj(droneSprite, glm::vec3(getRandomPos(), 0), glm::vec2(80.0f));
	GameObject shieldObj(shieldSprite, glm::vec3(getRandomPos(), 0), glm::vec2(60.0f));
	GameObject weaponObj(weaponSprite, glm::vec3(getRandomPos(), 0), glm::vec2(60.0f));

	Projectile sMissile(sMissileSprite, glm::vec3(getRandomPos(), 0.3f), glm::vec2(40.0f), 1.5f, explosionSprite,
		90.f, glm::vec3(1.f), glm::vec2(250.f, 0.f));


#pragma endregion

#pragma region Camera
	// // // // CAMERA STUFF // // // //
	
	Camera camera(glm::vec3(0.0f, 0.0f, 0.5f));

	//glm::mat4 projection;

	glm::mat4 projection = glm::ortho(0.0f, 800.f, 600.f, 0.0f);

	spriteShader.setMat4("projection", projection);

	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;

	SpriteRenderer renderer(spriteShader);

#pragma endregion

	glClearColor(0.0f, 0.6f, 0.3f, 1.0f);

	int start = SDL_GetTicks();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	SubTexture sprite = SubTexture::createFromIndexes(&shipTexture, glm::vec2(3, 0), glm::vec2(64, 64));

	Player player = generatePlayer(sprite);
	
	player.setMissile(&sMissile);

	TextRenderer textRenderer(&textShader, &fontTexture, glm::vec2(16.0f, 16.0f));
	Game game(&player, renderer, textRenderer, background, camera);
	
	game.objects.push_back(&mAsteroidObj);
	game.objects.push_back(&mAsteroid1Obj);
	game.objects.push_back(&sAsteroidObj);
	game.objects.push_back(&sAsteroid1Obj);
	game.objects.push_back(&sAsteroid2Obj);
	game.objects.push_back(&lonerObj);
	game.objects.push_back(&rusherObj);
	game.objects.push_back(&droneObj);
	game.objects.push_back(&shieldObj);
	game.objects.push_back(&weaponObj);
	game.objects.push_back(&sMissile);

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

		spriteShader.setMat4("view", camera.getView());
		
		
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

glm::vec2 getRandomPos()
{
	float minX = 300, maxX = 1600;
	float minY = 0, maxY = 500;

	float randomX = minX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxX - minX)));
	float randomY = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxY - minY)));
	return glm::vec2(randomX, randomY);
}
