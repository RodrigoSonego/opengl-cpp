#pragma once
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"
#include "Camera.h"
#include <glad/glad.h>

class SpriteRenderer
{
public:
	SpriteRenderer(Shader spriteShader);

	/// <summary>
	/// Renders a texture, can take a sprite index to then generate a SubTexture internally
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation"></param>
	/// <param name="spriteIndex">Sprite of the texture to be rendered, in case of a spritesheet</param>
	/// <param name="color"></param>
	void RenderSprite(Texture* tex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec2 spriteIndex = { 0, 0 }, glm::vec3 color = {1.f,1.f,1.f});
	/// <summary>
	/// Renders a texture, can take specific texture coordinates to be rendered.
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation"></param>
	/// <param name="texCoords"></param>
	/// <param name="color"></param>
	void RenderSprite(Texture* tex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec2 texCoords[4], glm::vec3 color = { 1.f,1.f,1.f });
	/// <summary>
	/// Renders a subtexture at specified position, with specified size and rotation
	/// </summary>
	/// <param name="subTex">SubTexture to be rendered</param>
	/// <param name="position">local position to be rendered</param>
	/// <param name="size">size of the rendered object</param>
	/// <param name="rotation">rotation on the Z axis</param>
	/// <param name="color">color of the sprite (white is default)</param>
	void RenderSprite(SubTexture subTex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec3 color = { 1.f,1.f,1.f });
	/// <summary>
	/// Renders a subtexture with specified model matrix and color
	/// </summary>
	/// <param name="subTex"></param>
	/// <param name="modelMatrix"></param>
	/// <param name="color"></param>
	void RenderSprite(SubTexture subTex, glm::mat4 modelMatrix, glm::vec3 color);


private:
	/// <summary>
	/// Sets up the buffers and shader parameters for rendering
	/// </summary>
	void setupRendering();

	Shader spriteShader;
	GLuint quadVAO, VBO, EBO;
};

