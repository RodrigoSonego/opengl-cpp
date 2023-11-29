#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "dependencies/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BufferController.h"
#include "BufferObject.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>
#include "FileReader.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window* startOpenGLWindow();
void captureMouse();
void stopCapturingMouse();
void processEscapePress();

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



	std::vector<glm::vec3> vertices;
	std::vector<GLushort>  indices;

	std::vector<Vertex> coolerVertices;

	FileReader reader;
	reader.load_obj("res/models/shiba.obj", coolerVertices, indices);

	BufferObject vboVert(coolerVertices.data(), coolerVertices.size() * sizeof(coolerVertices[0]), BufferObject::BufferType::Array);
	BufferObject eboVert(indices.data(), indices.size() * sizeof(indices[0]), BufferObject::BufferType::ElementArray);
	

	GLuint vaoVert;//, vaoCoord; // vertex array object
	glGenVertexArrays(1, &vaoVert);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(vaoVert);
	// 2. copy our vertices array in a buffer for OpenGL to use
	vboVert.Bind();
	eboVert.Bind();

	Shader shader("vertex.vert", "fragment.frag");

#pragma region Vertex Attribs
	// 3. then set our vertex attributes pointers

	vboVert.Bind();
	shader.enableVertexAttribArray("position", 3, 5, 0);
	
	//vboCoord.Bind();
	shader.enableVertexAttribArray("texCoord", 2, 5, 3);
#pragma endregion

#pragma region Texture

	Texture containerTex("res/textures/shiba.png", GL_RGB);

	//Texture faceTex("res/textures/awesomeface.png", GL_RGBA);

#pragma endregion
	

	shader.use();

	shader.setTextureIndex("outTexture", 0);
	//shader.setTextureIndex("outTexture2", 1);

	
#pragma region camera bagulhos
	
	glm::mat4 transMat(1.0f);

	shader.setMat4("transform", transMat);

	// // // // CAMERA STUFF // // // //
	
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


	glm::mat4 projection;
	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;


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
			//processMouseInput(windowEvent);
		}
		
		camera.moveWithKeyboard(deltaTime);
		processEscapePress();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.getView();
		shader.setMat4("view", view);



#pragma region Camera bagulhos no loop

		projection = camera.getPerspective(ratio);
		
		shader.setMat4("projection", projection);

#pragma endregion


		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.use();
		
		containerTex.bindTexture(0);

		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[0]);
		float angle = 45.0f * deltaTime;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));

		shader.setMat4("model", model);

		glBindVertexArray(vaoVert);
		vboVert.Bind();
		eboVert.Bind();
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

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
