#pragma once
#include <glad/glad.h>

class Texture
{
private:
	GLuint textureID;

public:
	Texture(const char* texturePath, GLint colorFormat);
	void bindTexture(int index);

	int width, height, nChannels;
};