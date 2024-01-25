#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Struct describing the position, size, and rotation of a 2D object
/// </summary>
struct Transform {
	glm::vec3 position;
	glm::vec2 size;
	float rotation; // only considering rotation on Z axis, since 2d game
};