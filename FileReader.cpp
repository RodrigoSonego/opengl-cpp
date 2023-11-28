#include "FileReader.h"
#include <fstream>
#include <sstream>
#include "dependencies/stb_image/stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glad/glad.h>

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

void FileReader::load_obj(const char* filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLushort>& elements)
{
	std::ifstream in(filename, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open " << filename << std::endl;
		exit(1);
	}

	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			glm::vec4 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			v.w = 1.0f;
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(2));
			GLushort a, b, c, ign;
			
			std::cout << s.str() << std::endl;

			s >> a;
			s.ignore();
			s >> ign;
			s.ignore();
			s >> ign;

			s >> b;
			s.ignore();
			s >> ign;
			s.ignore();
			s >> ign;

			s >> c;
			a--;
			b--;
			c--;

			std::cout << a << " " << b << " " << c << std::endl;
			elements.push_back(a);
			elements.push_back(b);
			elements.push_back(c);
		}
		/* anything else is ignored */
	}

	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i + 1];
		GLushort ic = elements[i + 2];
		if (ia <= vertices.size() && ib <= vertices.size() && ic <= vertices.size())
		{
			glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]), glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
			normals[ia] = normals[ib] = normals[ic] = normal;
		}
		else
		{
			break;
		}
	}
}