#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
class FileReader
{
public:
	char* readTextFile(const char* pathToFile);

	unsigned char* readImageFile(const char* pathToFile, int* width, int* height, int* nChannels) const;

	void load_obj(const char* filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLushort>& elements);
private:
	std::ifstream readFile(const char* pathToFile);
};

