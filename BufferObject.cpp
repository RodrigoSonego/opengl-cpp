#include "BufferObject.h"
#include <iostream>

BufferObject::BufferObject(const void* data, GLsizei size, BufferType type) {
    glGenBuffers(1, &id);
    glBindBuffer(static_cast<GLenum>(type), id);
    glBufferData(static_cast<GLenum>(type), size, data, GL_STATIC_DRAW);

    this->type = type;
}

void BufferObject::Bind() const {
    glBindBuffer(static_cast<GLenum>(type), id);
}

void BufferObject::Unbind() const {
    glBindBuffer(static_cast<GLenum>(type), 0);
}