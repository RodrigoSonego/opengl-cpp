#version 330 core

in vec3 position;
in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = texCoord;
	gl_Position = projection * view * model * transform * vec4(position, 1.0);
}