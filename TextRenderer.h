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

	/// <summary>
	/// Iterates through chars of the text to render the text on screen at the specified position
	/// </summary>
	/// <param name="text">: Text to be rendered with font</param>
	/// <param name="position">: Text position</param>
	/// <param name="scale">: Text scale</param>
	/// <param name="color">: Text color</param>
	void renderText(std::string text, glm::vec2 position, float scale, glm::vec3 color);

private:
	/// <summary>
	/// Sets up buffers necessary for rendering
	/// </summary>
	void setupRenderer();
	/// <summary>
	/// Maps a bitmap font based on the sprite size and first and last char value
	/// (currently hardcoded to the xenon bitmap font)
	/// </summary>
	void mapFont();

	int rowCount, colCount;

	glm::vec2 m_GlyphSize;
	Shader* textShader;
	Texture* m_FontTexure;

	std::map<char, glm::vec2> glyphCoords;

	GLuint VBO, EBO, quadVAO;
};

