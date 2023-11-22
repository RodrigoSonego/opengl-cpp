#include "FileReader.h"
#include <fstream>
#include <sstream>
#include "dependencies/stb_image/stb_image.h"
#include <iostream>

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
