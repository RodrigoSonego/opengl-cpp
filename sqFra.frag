#version 330 core
in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D outTexture;
uniform sampler2D outTexture2;


void main()
{
	vec4 colTex1 = texture(outTexture, TexCoord);
	vec4 colTex2 = texture(outTexture2, TexCoord);
	outColor = mix(colTex1, colTex2, 0.5);
}