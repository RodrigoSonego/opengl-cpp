#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	/// <summary>
///		Used to enable a single vertex attrib array, suposes values are floats
/// </summary>
/// <param name="name">Attribute name</param>
/// <param name="size">Size of array</param>
/// <param name="stride">Space between consecutive vertex attributes</param>
/// <param name="offset">Offset where data begins in buffer</param>
	void enableVertexAttribArray(const char* name, int size, int stride, int offset);

	void setMat4(const char* name, glm::mat4 mat) const;


private:
	void compileAndLinkShader(const char* vertexCode, const char* fragmentCode);
};

