#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class BufferObject {
public:
    /// <summary>
    /// Enum for better readability of Array buffer types
    /// </summary>
    enum class BufferType {
        Array = GL_ARRAY_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER
    };

    BufferObject(const void* data, GLsizei size, BufferType bufferType);

    void Bind() const;
    void Unbind() const;

private:
    GLuint id;
    BufferType type;
};