#include "SubTexture.h"
#include "iostream"

SubTexture::SubTexture(Texture* tex, glm::vec2 spriteCoord, glm::vec2 spriteSize)
    : texture(tex), m_SpritePosition(spriteCoord), m_Size(spriteSize)
{
    calculateCoords();
}

SubTexture SubTexture::createFromIndexes(Texture* tex, glm::vec2 spriteIndices, glm::vec2 spriteSize)
{
    return SubTexture(tex, spriteIndices, spriteSize);
}

glm::vec2* SubTexture::getTexCoords()
{
    return texCoords;
}

glm::vec2 SubTexture::getSpritePosition()
{
    return m_SpritePosition;
}

Texture* SubTexture::getTexture()
{
    return texture;
}

glm::vec2 SubTexture::getSize()
{
    return m_Size;
}

void SubTexture::updateSpritePosition(glm::vec2 position)
{
    float maxX = (texture->width / m_Size.x) - 1;
    float maxY = (texture->height / m_Size.y) - 1;

    if (position.x > maxX) { position = glm::vec2(0, position.y+1); }
    if (position.y > maxY) { position = glm::vec2(0, 0); }

    m_SpritePosition = position;

    calculateCoords();
}

void SubTexture::switchToNextSprite(bool isBackwards)
{
    int offset = isBackwards ? -1 : 1;
    float nextX = m_SpritePosition.x + offset;

    float maxX = (texture->width / m_Size.x) - 1;
    float maxY = (texture->height / m_Size.y) - 1;

    glm::vec2 newPosition(nextX, m_SpritePosition.y);

    if (newPosition.x > maxX) { newPosition = glm::vec2(0, newPosition.y + 1); }
    if (newPosition.y > maxY) { newPosition = glm::vec2(0, 0); }
    if (newPosition.x < 0) { newPosition = glm::vec2(maxX, newPosition.y - 1); }
    if (newPosition.y < 0) { newPosition = glm::vec2(maxX, maxY); }

    m_SpritePosition = newPosition;
    calculateCoords();
}

int SubTexture::getMaxNumberOfFrames()
{
    int cols = texture->width / m_Size.x;
    int rows = texture->height / m_Size.y;
    return cols * rows;
}

void SubTexture::calculateCoords()
{
    glm::vec2 minCoord = { (m_SpritePosition.x * m_Size.x) / texture->width, 
                        (m_SpritePosition.y * m_Size.y) / texture->height };

    glm::vec2 maxCoord = { ((m_SpritePosition.x + 1) * m_Size.x) / texture->width, 
                    ((m_SpritePosition.y + 1) * m_Size.y) / texture->height };

    texCoords[0] = { maxCoord.x, maxCoord.y };
    texCoords[1] = { maxCoord.x, minCoord.y };
    texCoords[2] = { minCoord.x, maxCoord.y };
    texCoords[3] = { minCoord.x, minCoord.y };
}
