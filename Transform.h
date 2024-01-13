#pragma once

#include <glm/glm.hpp>
struct Transform {
	glm::vec3 position;
	glm::vec2 size;
	float rotation; // only considering rotation on Z axis, since 2d game
};