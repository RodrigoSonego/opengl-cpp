#pragma once

#include <glad/glad.h>

class BufferController
{
private:
	GLuint elementBuffer;
	GLuint vertexBuffer;
	GLuint vertexArrayObject; //not a buffer but will live here for now

public:
	GLuint generateBuffer(const void *data);
	void bindBuffer(GLuint bufferID) const;
	GLuint generateVertexArray(GLsizei quantity);
};

