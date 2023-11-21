#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "dependencies/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BufferController.h"
#include "Shader.h"
#include "BufferObject.h"

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

bool couldCompileShader(GLuint& shaderToVerify, char(&outInfoLog)[512]);
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
	// Vertices (Position (vec2), Color (vec3))
	//float vertices[] = {
	//	  -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
	//	 -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y)
	//	  0.0f,  -1.0f, 0.0f, 0.0f, 1.0f  // Vertex 3 (X, Y)
	//	// 0.7f,  0.0f,  // Vertex 3 (X, Y)
	//	//  0.0f, -0.3f // Vertex 1 (X, Y)
	//	//-0.7f,  0.0f, // Vertex 2 (X, Y)
	//};

	// square vertices
	float vertices[] = {
	// X, Y			// r, g, b			//// Texture X, Y
	0.5f,  0.5f,    1.0f, 1.0f, 0.0f,   // 1.0f, 1.0f, // top right
    0.5f, -0.5f,    1.0f, 1.0f, 0.0f,   // 1.0f, 0.0f, // bottom right
   -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   // 0.0f, 0.0f, // bottom left
   -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,   // 0.0f, 1.0f// top left
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	/*float vertices[] = {
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
	};*/

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

	Shader shader("sqrVer.vert", "sqFra.frag");

#pragma region Vertex Attribs
	// 3. then set our vertex attributes pointers

	shader.enableVertexAttribArray("position", 2, 5, 0);
	
	shader.enableVertexAttribArray("color", 3, 5, 2);
	//shader.enableVertexAttribArray("texCoord", 2, 5, 3);
#pragma endregion

	/*
#pragma region Texture

	stbi_set_flip_vertically_on_load(true);

	GLuint containerTex;
	glGenTextures(1, &containerTex);
	glBindTexture(GL_TEXTURE_2D, containerTex);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	unsigned char* data = stbi_load("res/textures/container.jpg", &width, &height, &nChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture!";
	}

	stbi_image_free(data);


	GLuint faceTex;
	glGenTextures(1, &faceTex);
	glBindTexture(GL_TEXTURE_2D, faceTex);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("res/textures/awesomeface.png", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

#pragma endregion
	*/

	shader.use();

	//GLint tex1Location = glGetUniformLocation(shaderProgram, "outTexture");
	//GLint tex2Location = glGetUniformLocation(shaderProgram, "outTexture2");

	//glUniform1i(tex1Location, 0);
	//glUniform1i(tex2Location, 1);
	/*
	
#pragma region camera bagulhos
	glm::mat4 transMat(1.0f);
	//transMat = glm::rotate(transMat, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//transMat = glm::scale(transMat, glm::vec3(0.5, 0.5, 0.5));

	shader.setMat4("transform", transMat);
	//GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMat));

	// // // // CAMERA STUFF // // // //
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));


	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraTarget, worldUp);


	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

	//glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;
	projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);

	//GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	shader.setMat4("model", model);

	//GLuint viewLocation = glGetUniformLocation(shaderProgram, "view");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	shader.setMat4("view", view);

	//GLuint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	//glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
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
	*/


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


		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 2.0f);
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

/*
#pragma region Camera bagulhos no loop
		shader.setMat4("view", view);

		projection = glm::perspective(glm::radians(fov), ratio, 0.1f, 100.0f);
		//glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
		shader.setMat4("projection", projection);

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

#pragma endregion
		*/


		//glClear(GL_COLOR_BUFFER_BIT);
		
		//glUseProgram(shaderProgram);
		
		/*
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTex);		
		*/

		shader.use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

bool couldCompileShader(GLuint &shaderToVerify, char(&outInfoLog)[512]) {
	GLint success;
	glGetShaderiv(shaderToVerify, GL_COMPILE_STATUS, &success); // checks for shader compile status

	if (!success) {
		glGetShaderInfoLog(shaderToVerify, 512, NULL, outInfoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << outInfoLog << std::endl;
	}

	return success;
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