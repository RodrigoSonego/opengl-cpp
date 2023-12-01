#include "FileReader.h"
#include <fstream>
#include <sstream>
#include "dependencies/stb_image/stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glad/glad.h>
#include "Vertex.h"

char* FileReader::readTextFile(const char* pathToFile)
{
	std::ifstream file;

	std::string fileContent;

	try {
		std::stringstream textStream;

		file.open(pathToFile);

		textStream << file.rdbuf();

		file.close();

		fileContent = textStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::TEXT_FILE_NOT_SUCCESFULLY_READ_AT " << pathToFile << std::endl;
		return nullptr;
	}

    return _strdup(fileContent.c_str());
}

unsigned char* FileReader::readImageFile(const char* pathToFile, int* width, int* height, int* nChannels) const
{
	unsigned char* data = stbi_load(pathToFile, width, height, nChannels, 0);

	if (!data) {
		std::cout << "Failed to load texture at " << pathToFile;
		return nullptr;
	}

	return data;
}

void FileReader::load_obj(const char* filename, std::vector<Vertex>& outVertices, std::vector<GLushort>& outIndices)
{
	std::ifstream in(filename, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open " << filename << std::endl;
		exit(1);
	}

	// Temporary arrays to store positions and texCoords as read.
	std::vector<glm::vec3> tempPositions;
	std::vector<glm::vec2> tempCoords;

	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream stream(line.substr(2));
			glm::vec3 tempVertex;
			stream >> tempVertex.x;
			stream >> tempVertex.y;
			stream >> tempVertex.z;

			tempPositions.push_back(tempVertex);
		}
		else if (line.substr(0, 2) == "vt") {
			std::istringstream stream(line.substr(2));
			glm::vec2 tempCoord;

			stream >> tempCoord.x;
			stream >> tempCoord.y;

			//tempCoord.y = 1 - tempCoord.y;
			tempCoords.push_back(tempCoord);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream stream(line.substr(2));
			GLushort posIndex, ign;
			GLushort texIndex;

			// Line is 00/00/00 00/00/00 00/00/00
			// Each iteration goes through one of the three sets
			for (short i = 0; i < 3; i++)
			{
				// grab the first element and put it in posIndex
				stream >> posIndex;
				//ignore second element ( / )
				stream.ignore();
				// grab the third element and put it in texIndex
				stream >> texIndex;
				//ignore fourth element ( / )
				stream.ignore();
				// throw the rest into a dummy variable and continue reading the line
				stream >> ign;

				--posIndex;
				--texIndex;

				//////// Debug logs
				//std::cout << "tempVer size: " << tempPositions.size() << " tempCoord: " << tempCoords.size() << " vertices: " << vertices.size() << std::endl;
				//std::cout << "posIndex: " << posIndex << " texIndex: " << texIndex << std::endl;
				
				// If an object doesn't have texture coordinates it is considered in a bad format
				if (tempCoords.size() == 0) {
					std::cout << "ERROR::OBJ::BAD:FORMAT::ABORTING";
					return;
				}

				Vertex tempVert;
				tempVert.position = tempPositions[posIndex];
				tempVert.texCoord = tempCoords[texIndex];

				// Remap indexes to account for more than one UV coordinate
				// using the same position, essentialy duplicating some vertices
				GLushort newIndex = outVertices.size();
				outVertices.push_back(tempVert);
				outIndices.push_back(newIndex);

			}
		}
		/* anything else is ignored */
	}

	
	
}