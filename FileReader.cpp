#include "FileReader.h"
#include <fstream>
#include <sstream>

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

//std::ifstream FileReader::readFile(const char* pathToFile)
//{
    

//}
