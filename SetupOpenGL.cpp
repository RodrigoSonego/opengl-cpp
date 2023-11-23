#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "dependencies/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BufferController.h"
#include "BufferObject.h"
#include "Shader.h"
#include "Texture.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// gore
float lastX = 400, lastY = 300;
float pitch;
float yaw = -90.0f;

float fov = 45.0f;

SDL_Window* startOpenGLWindow();
void processMouseInput(SDL_Event ev);
static void processKeyboard(float deltaTime);

int main(int argc, char** argv)
{
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


	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	BufferObject vbo(vertices, sizeof(vertices), BufferObject::BufferType::Array);
	BufferObject ebo(indices, sizeof(indices), BufferObject::BufferType::ElementArray);


	GLuint vao; // vertex array object
	glGenVertexArrays(1, &vao);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(vao);

	// 2. copy our vertices array in a buffer for OpenGL to use
	vbo.Bind();
	ebo.Bind();

	Shader shader("vertex.vert", "fragment.frag");

#pragma region Vertex Attribs
	// 3. then set our vertex attributes pointers

	shader.enableVertexAttribArray("position", 3, 5, 0);
	
	shader.enableVertexAttribArray("texCoord", 2, 5, 3);
#pragma endregion

#pragma region Texture

	Texture containerTex("res/textures/container.jpg", GL_RGB);

	Texture faceTex("res/textures/awesomeface.png", GL_RGBA);

#pragma endregion
	

	shader.use();

	shader.setTextureIndex("outTexture", 0);
	shader.setTextureIndex("outTexture2", 1);

	
#pragma region camera bagulhos
	
	glm::mat4 transMat(1.0f);

	shader.setMat4("transform", transMat);

	// // // // CAMERA STUFF // // // //
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));


	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraTarget, worldUp);


	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;
	projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);

	shader.setMat4("model", model);

	shader.setMat4("view", view);

	shader.setMat4("projection", projection);


	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};


#pragma endregion
	


	glClearColor(0.0f, 0.6f, 0.3f, 1.0f);

	int start = SDL_GetTicks();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	SDL_Event windowEvent;
	while (true)
	{
		int now = SDL_GetTicks();

		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;

		// Event loop
		while (SDL_PollEvent(&windowEvent) != 0) {
			if (windowEvent.type == SDL_QUIT) break;

			//processMouseInput(windowEvent);
			//processKeyboard(deltaTime);
		}
		
		//SDL_ShowCursor(SDL_DISABLE);
		//SDL_CaptureMouse(SDL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		shader.setMat4("view", view);


#pragma region Camera bagulhos no loop

		projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
		
		shader.setMat4("projection", projection);

#pragma endregion


		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.use();
		

		containerTex.bindTexture(0);
		faceTex.bindTexture(1);

		glBindVertexArray(vao);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
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

void processMouseInput(SDL_Event ev)
{
	if (ev.type == SDL_MOUSEMOTION) {
		float xPos = ev.button.x;
		float yPos = ev.button.y;

		float xOffset = xPos - lastX;
		float yOffset = lastY - yPos;

		lastX = xPos;
		lastY = yPos;

		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		static bool firstMouse = true;
		if (firstMouse)
		{
			firstMouse = false;
			lastX = xPos;
			lastY = yPos;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
	}

	if (ev.type == SDL_MOUSEWHEEL)
	{
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= ev.wheel.y;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}
}

void processKeyboard(float deltaTime)
{
	float cameraSpeed = 4.f * deltaTime; // adjust accordingly
	const Uint8* keyState;
	keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyState[SDL_SCANCODE_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}