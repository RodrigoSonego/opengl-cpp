#pragma once

#include <iostream>
class FileReader
{
public:
	char* readTextFile(const char* pathToFile);

private:
	std::ifstream readFile(const char* pathToFile);
};

