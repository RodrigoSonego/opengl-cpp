#pragma once
#include <glm/glm.hpp>

/// <summary>
/// Simple struct to describe a vertex with 3-dimensional positions and 2-dimensional texture coordinates
/// </summary>
struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};
