#pragma once
#include <glm/glm.hpp>
#include "Texture.h"

class SubTexture
{
public:
	SubTexture(Texture* tex, glm::vec2 spriteCoord, glm::vec2 spriteSize);

	static SubTexture createFromIndexes(Texture* tex, glm::vec2 spriteIndices, glm::vec2 spriteSize);
	
	glm::vec2* getTexCoords();
	glm::vec2 getSpritePosition();
	Texture* getTexture();
	glm::vec2 getSize();
	void updateSpritePosition(glm::vec2 position);
	void switchToNextSprite(bool forward = true);
	int getMaxNumberOfFrames();

private:
	Texture* texture;

	void calculateCoords();

	glm::vec2 m_SpritePosition;
	glm::vec2 m_Size;
	glm::vec2 texCoords[4];
};

