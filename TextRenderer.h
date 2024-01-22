#pragma once
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>
#include <map>

#define FIRST_CHAR 32
#define LAST_CHAR 125

class TextRenderer
{
public:
	TextRenderer(Shader* shader, Texture* texture, glm::vec2 glyphSize);

	void renderText(std::string text, glm::vec2 position, float scale, glm::vec3 color);

private:
	void setupRenderer();
	void mapFont();

	int rowCount, colCount;

	glm::vec2 m_GlyphSize;
	Shader* textShader;
	Texture* m_FontTexure;

	std::map<char, glm::vec2> glyphCoords;

	GLuint VBO, EBO, quadVAO;
};

