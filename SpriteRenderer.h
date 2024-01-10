#pragma once
#include "Shader.h"
#include "Texture.h"
#include <glad/glad.h>

class SpriteRenderer
{
public:
	SpriteRenderer(Shader spriteShader);

	void RenderSprite(Texture tex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec2 spriteIndex = { 0, 0 }, glm::vec3 color = {1.f,1.f,1.f});

private:
	void setupRendering();
	void updateTextureCoordinates(glm::vec2* newTexCoords);

	Shader spriteShader;
	GLuint quadVAO, VBO, EBO;
};

