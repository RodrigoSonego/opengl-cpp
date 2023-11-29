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


	std::vector<Vertex> vertices;
	std::vector<GLushort>  indices;


	FileReader reader;
	reader.load_obj("res/models/rayman.obj", vertices, indices);

	BufferObject vbo(vertices.data(), vertices.size() * sizeof(vertices[0]), BufferObject::BufferType::Array);
	BufferObject ebo(indices.data(), indices.size() * sizeof(indices[0]), BufferObject::BufferType::ElementArray);
	

	GLuint vao; // Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Binding buffers to the VAO
	vbo.Bind();
	ebo.Bind();

	Shader shader("vertex.vert", "fragment.frag");

#pragma region Vertex Attribs
	
	shader.enableVertexAttribArray("position", 3, 5, 0);
	shader.enableVertexAttribArray("texCoord", 2, 5, 3);

#pragma endregion

#pragma region Texture
	// Create and bind texture afterwards
	Texture mainTexture("res/textures/rayman.png", GL_RGB);

	shader.use();

	shader.setTextureIndex("outTexture", 0);
	mainTexture.bindTexture(0);

#pragma endregion
	


	glm::mat4 transMat(1.0f);

	shader.setMat4("transform", transMat);

#pragma region Camera
	// // // // CAMERA STUFF // // // //
	
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	glm::mat4 projection;
	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;


#pragma endregion


	glClearColor(0.0f, 0.6f, 0.3f, 1.0f);

	int start = SDL_GetTicks();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	captureMouse();

	SDL_Event windowEvent;
	while (true)
	{
		int now = SDL_GetTicks();

		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;

		// Event loop
		while (SDL_PollEvent(&windowEvent) != 0) {
			if (windowEvent.type == SDL_QUIT) break;

			camera.processMouseInput(windowEvent);
		}
		
		camera.moveWithKeyboard(deltaTime);
		processEscapePress();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Camera setup on loop
		glm::mat4 view = camera.getView();
		shader.setMat4("view", view);

		projection = camera.getPerspective(ratio);
		
		shader.setMat4("projection", projection);
#pragma endregion

		


#pragma region Position and Rotate object

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0,0,0));
		float angle = 45.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		shader.setMat4("model", model);

		transMat = glm::rotate(transMat, deltaTime, glm::vec3(0.0f, 0.3f, 0.0f));
		shader.setMat4("transform", transMat);

#pragma endregion


		int bufferSize;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	
		// Get number of elements to draw based on buffer size
		glDrawElements(GL_TRIANGLES, bufferSize / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

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
