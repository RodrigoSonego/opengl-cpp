#include "BufferController.h"
#include <iostream>

GLuint BufferController::generateBuffer(const void* data)
{
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	
	//elementBuffer = ebo;

    return 1;
}

void BufferController::bindBuffer(GLuint bufferID) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
}
