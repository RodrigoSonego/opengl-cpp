#include <iostream>
#include <glad/glad.h>
#include <SDL.h>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
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
	float vertices[] = {
		  -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
		 -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y)
		  0.0f,  -1.0f, 0.0f, 0.0f, 1.0f  // Vertex 3 (X, Y)
		// 0.7f,  0.0f,  // Vertex 3 (X, Y)
		//  0.0f, -0.3f // Vertex 1 (X, Y)
		//-0.7f,  0.0f, // Vertex 2 (X, Y)
	};

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
		out vec3 Color;

		void main()
		{
			Color = color;
			gl_Position = vec4(position, 0.0, 1.0);
		}
		)glsl";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	//check for errors
	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // checks for shader compile status

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader stuff

	const char* fragmentShaderSource = R"glsl(#version 330 core
	in vec3 Color;
	out vec4 outColor;

	void main()
	{
		outColor = vec4(Color, 1.0f);
	}
	)glsl";

	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for errors AGAIN
	// TODO: Turn to method
	GLint success2;
	char infoLog2[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2); // checks for shader compile status

	if (!success2) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 3. then set our vertex attributes pointers
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colorAttrib);										// offset, color starts after 2 floats				
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 2* sizeof(float) ));

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//int start = SDL_GetTicks();

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}
		
		//int now = SDL_GetTicks();
		//float time = (now - start) / 1000.0f;
		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 2.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}