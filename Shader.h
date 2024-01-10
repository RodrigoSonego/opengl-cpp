#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int id;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="vertexPath">path to the vertex shader</param>
	/// <param name="fragmentPath">path to the fragment shader</param>
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader() { }
	void use();

	/// <summary>
///		Used to enable a single vertex attrib array, suposes values are floats
/// </summary>
/// <param name="name">Attribute name</param>
/// <param name="size">Size of array</param>
/// <param name="stride">Space between consecutive vertex attributes</param>
/// <param name="offset">Offset where data begins in buffer</param>
	void enableVertexAttribArray(const char* name, int size, int stride, int offset);

	/// <summary>
	/// Set a mat4 value on the shader
	/// </summary>
	/// <param name="name">property name</param>
	/// <param name="mat">matrix value to be set</param>
	void setMat4(const char* name, glm::mat4 mat) const;

	void setVec2Array(const char* name, const glm::vec2* values, int count);
	void setVec3f(const char* name, glm::vec3 mat) const;


	/// <summary>
	/// Attributes a texture uniform to a texture index
	/// </summary>
	/// <param name="name">name of the texture</param>
	/// <param name="index">index to be attributed</param>
	void setTextureIndex(const char* name, int index) const;

private:
	void compileAndLinkShader(const char* vertexCode, const char* fragmentCode);
};

