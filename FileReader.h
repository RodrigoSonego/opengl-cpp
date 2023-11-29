#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Vertex.h"

class FileReader
{
public:
	char* readTextFile(const char* pathToFile);

	unsigned char* readImageFile(const char* pathToFile, int* width, int* height, int* nChannels) const;

	void load_obj(const char* filename, std::vector<Vertex>& outVertices, std::vector<GLushort>& outIndices);
private:
	std::ifstream readFile(const char* pathToFile);
};

