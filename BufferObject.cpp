#include "BufferObject.h"
#include <iostream>

BufferObject::BufferObject(const void* data, GLsizei size, BufferType type) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(static_cast<GLenum>(type), m_RendererID);
    glBufferData(static_cast<GLenum>(type), size, data, GL_STATIC_DRAW);

    this->type = type;
}

void BufferObject::Bind() const {
    glBindBuffer(static_cast<GLenum>(type), m_RendererID);
}

void BufferObject::Unbind() const {
    glBindBuffer(static_cast<GLenum>(type), 0);
}