#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

class SubTexture
{
public:
	SubTexture(Texture& tex, glm::vec2 minCoord, glm::vec2 maxCoord, glm::vec2 spriteCoord);

	static SubTexture createFromIndexes(Texture& tex, glm::vec2 spriteIndices, glm::vec2 spriteSize);
	
	glm::vec2* getTexCoords();
	glm::vec2 getSpritePosition();
	Texture& getTexture();
	glm::vec2 getSize();

private:
	Texture& texture;

	glm::vec2 m_SpritePosition;
	glm::vec2 size;
	glm::vec2 texCoords[4];
};

