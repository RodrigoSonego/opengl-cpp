#pragma once
#include <glad/glad.h>

class Texture
{
private:
	GLuint textureID;
	int width, height, nChannels;

public:
	Texture(const char* texturePath, GLint colorFormat);
	void bindTexture(int index);
};