#include "SubTexture.h"

SubTexture::SubTexture(Texture& tex, glm::vec2 minCoord, glm::vec2 maxCoord, glm::vec2 spriteCoord)
    : texture(tex), m_SpritePosition(spriteCoord)
{
    texCoords[0] = { maxCoord.x, maxCoord.y };
    texCoords[1] = { maxCoord.x, minCoord.y };
    texCoords[2] = { minCoord.x, maxCoord.y };
    texCoords[3] = { minCoord.x, minCoord.y };

    size.x = maxCoord.x - minCoord.x;
    size.y = maxCoord.y - minCoord.y;
}

SubTexture SubTexture::createFromIndexes(Texture& tex, glm::vec2 spriteIndices, glm::vec2 spriteSize)
{
    glm::vec2 minCoord = { (spriteIndices.x * spriteSize.x) / tex.width, (spriteIndices.y * spriteSize.y) / tex.height };
    glm::vec2 maxCoord = { ((spriteIndices.x + 1) * spriteSize.y) / tex.width, ((spriteIndices.y + 1) * spriteSize.y) / tex.height };

    return SubTexture(tex, minCoord, maxCoord, spriteIndices);
}

glm::vec2* SubTexture::getTexCoords()
{
    return texCoords;
}

glm::vec2 SubTexture::getSpritePosition()
{
    return m_SpritePosition;
}

Texture& SubTexture::getTexture()
{
    return texture;
}

glm::vec2 SubTexture::getSize()
{
    return size;
}
