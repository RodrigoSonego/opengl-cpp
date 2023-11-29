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
			GLushort posIndex, b, c, ign;
			GLushort texIndex, uvB, uvC;
			
			std::cout << stream.str() << std::endl;

			for (short i = 0; i < 3; i++)
			{
				stream >> posIndex;
				stream.ignore();
				stream >> texIndex;
				stream.ignore();
				stream >> ign;

				--posIndex;
				--texIndex;

				//uvIndices.push_back(texIndex);
				//std::cout << "tempVer size: " << tempPositions.size() << " tempCoord: " << tempCoords.size() << " vertices: " << vertices.size() << std::endl;
				//std::cout << "posIndex: " << posIndex << " texIndex: " << texIndex << std::endl;
				
				if (tempCoords.size() == 0) {
					std::cout << "ERROR::OBJ::BAD:FORMAT::ABORTING";
					return;
				}

				Vertex tempVert;
				tempVert.position = tempPositions[posIndex];
				tempVert.texCoord = tempCoords[texIndex];

				GLushort newIndex = outVertices.size();

				outVertices.push_back(tempVert);

				outIndices.push_back(newIndex);
			}
		}
		/* anything else is ignored */
	}

	
	
}