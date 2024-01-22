#version 330 core
layout (location = 1) in vec3 position;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

uniform vec2 texCoords[4]; // Uniform array for texture coordinates

void main()
{
    TexCoords = texCoords[gl_VertexID];
    gl_Position = projection * model * vec4(position, 1.0);
}