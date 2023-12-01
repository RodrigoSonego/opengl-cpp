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

	/// <summary>
	/// Loads exclusively a .obj file into memory. Returns data through the out parameters
	/// </summary>
	/// <param name="filename">path to the .obj file</param>
	/// <param name="outVertices">Vector for the vertices to be saved</param>
	/// <param name="outIndices">Vector for the indices to be saved</param>
	void load_obj(const char* filename, std::vector<Vertex>& outVertices, std::vector<GLushort>& outIndices);

};

