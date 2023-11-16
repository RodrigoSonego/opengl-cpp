#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setUniform(const char* name, glm::f32 value) const;

private:
	void compileAndLinkShader(const char* vertexCode, const char* fragmentCode);
};

