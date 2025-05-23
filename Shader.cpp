#include "Shader.h"
#include "FileReader.h"
#include "glad/glad.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{	
	FileReader reader;
	
	char* vertexCode = reader.readTextFile(vertexPath);

	char* fragmentCode = reader.readTextFile(fragmentPath);

	compileAndLinkShader(vertexCode, fragmentCode);
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::enableVertexAttribArray(const char* name, int size, int stride, int offset)
{
	GLint attribLocation = glGetAttribLocation(id, name);
	glEnableVertexAttribArray(attribLocation);
	glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
}

void Shader::setMat4(const char* name, glm::mat4 mat) const
{
	GLuint transformLoc = glGetUniformLocation(id, name);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec2Array(const char* name, const glm::vec2* values, int count)
{
	glUniform2fv(glGetUniformLocation(id, name), count, &values[0][0]);
}

void Shader::setVec3f(const char* name, glm::vec3 mat) const
{
	glUniform3f(glGetUniformLocation(id, name), mat.x, mat.y, mat.z);
}

void Shader::setTextureIndex(const char* name, int index) const
{
	GLint location = glGetUniformLocation(id, name);

	glUniform1i(location, index);
}

void Shader::compileAndLinkShader(const char* vertexCode, const char* fragmentCode)
{
	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
