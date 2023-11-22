#pragma once

#include <iostream>
class FileReader
{
public:
	char* readTextFile(const char* pathToFile);

	unsigned char* readImageFile(const char* pathToFile, int* width, int* height, int* nChannels) const;

private:
	std::ifstream readFile(const char* pathToFile);
};

