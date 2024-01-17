#pragma once
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class TextRenderer
{
public:
	TextRenderer(Shader* shader, Texture* texture, glm::vec2 glyphSize);

	void renderText(std::string text, glm::vec2 position, float scale, glm::vec3 color);

private:
	void setupRenderer();

	int rowCount, colCount;

	glm::vec2 m_GlyphSize;
	Shader* textShader;
	Texture* m_FontTexure;

	GLuint VBO, EBO, quadVAO;
};

