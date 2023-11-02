#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "dependencies/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


bool couldCompileShader(GLuint& shaderToVerify, char(&outInfoLog)[512]);
SDL_Window* startOpenGLWindow();

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
	// X, Y			// r, g, b			// Texture X, Y
	0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, // top right
    0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, // bottom right
   -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // bottom left
   -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f// top left
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint ebo; // Element buffer object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint vbo; // Vertex buffer object
	glGenBuffers(1, &vbo); // Generate 1 buffer, store its id in vbo variable
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind vbo as a GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao; // vertex array object
	glGenVertexArrays(1, &vao);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(vao);

	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const char* vertexShaderSource = R"glsl(
		#version 330 core

		in vec2 position;
		in vec3 color;
		in vec2 texCoord;

		out vec3 Color;
		out vec2 TexCoord;

		uniform mat4 transform;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			Color = color;
			TexCoord = texCoord;
			gl_Position = projection * view * model * transform * vec4(position, 0.0, 1.0);
		}
		)glsl";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	char infoLog[512];
	bool couldCompileVert = couldCompileShader(vertexShader, infoLog);
	if (!couldCompileVert) {
		std::cout << "COULD NOT COMPILE VERTEX SHADERS";
	}
	// Fragment shader stuff

	const char* fragmentShaderSource = R"glsl(#version 330 core
	in vec3 Color;
	in vec2 TexCoord;

	out vec4 outColor;

	uniform sampler2D outTexture;
	uniform sampler2D outTexture2;


	void main()
	{
		vec4 colTex1 = texture(outTexture, TexCoord);
		vec4 colTex2 = texture(outTexture2, TexCoord);
		outColor = mix(colTex1, colTex2, 0.5);
	}
	)glsl";

	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//retutilizing infolog cause dont care
	bool couldCompileFrag = couldCompileShader(fragmentShader, infoLog);
	if (!couldCompileFrag) {
		std::cout << "COULD NOT COMPILE FRAGMENT SHADERS";
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

#pragma region Vertex Attribs
	// 3. then set our vertex attributes pointers
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colorAttrib);										// offset, color starts after 2 floats				
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)( 2* sizeof(float) ));

	GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "texCoord");
	glEnableVertexAttribArray(texCoordAttrib);
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

#pragma endregion

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

	glUseProgram(shaderProgram);

	GLint tex1Location = glGetUniformLocation(shaderProgram, "outTexture");
	GLint tex2Location = glGetUniformLocation(shaderProgram, "outTexture2");

	glUniform1i(tex1Location, 0);
	glUniform1i(tex2Location, 1);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glm::mat4 transMat(1.0f);
	//transMat = glm::rotate(transMat, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//transMat = glm::scale(transMat, glm::vec3(0.5, 0.5, 0.5));

	GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMat));

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	float ratio = SCREEN_WIDTH / SCREEN_HEIGHT;
	projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);

	GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	GLuint viewLocation = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glClearColor(0.0f, 0.6f, 0.3f, 1.0f);

	int start = SDL_GetTicks();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}
		
		int now = SDL_GetTicks();
		float time = (now - start) / 1000.0f;
		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 2.0f);
		

		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTex);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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