#include "TextRenderer.h"
#include "SubTexture.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

TextRenderer::TextRenderer(Shader* shader, Texture* texture, glm::vec2 glyphSize)
	: textShader(shader), m_FontTexure(texture), m_GlyphSize(glyphSize)
{
    colCount = texture->width / glyphSize.x;
    rowCount = texture->height / glyphSize.y;

    setupRenderer();

    std::cout << "text vbo: " << VBO << std::endl;
}


void TextRenderer::renderText(std::string text, glm::vec2 position, float scale, glm::vec3 color)
{
	SubTexture subtex = SubTexture::createFromIndexes(m_FontTexure, glm::vec2(0.0f), m_GlyphSize);

    int glyphX, glyphY, charCount = 0;

    textShader->use();

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c) {
        glyphX = (int)*c % colCount;
        glyphY = round( (float)*c / rowCount );

        subtex.updateSpritePosition(glm::vec2(glyphX, glyphY));

        float letterX = m_GlyphSize.x * (float)charCount * scale;
        
        glm::mat4 model(1.0f);
        glm::vec2 pos = position +glm::vec2(letterX, 0.0f);
        model = glm::translate(model, glm::vec3(pos, 0.0f));
        model = glm::scale(model, glm::vec3(m_GlyphSize.x * scale, m_GlyphSize.y * scale, 1.0));

        std::cout << *c << ": x= " << subtex.getSpritePosition().x << " y= " << subtex.getSpritePosition().y << std::endl;


        textShader->setMat4("model", model);
        textShader->setVec3f("spriteColor", color);

        textShader->setVec2Array("texCoords", subtex.getTexCoords(), 4);

        glBindVertexArray(this->quadVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ++charCount;
    }
}

void TextRenderer::setupRenderer()
{
    float vertices[] = {
        // pos
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

    int indices[] = {
        0, 2, 1,
        2, 3, 1
    };

    textShader->use();

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Position attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    textShader->setTextureIndex("image", 1);
    m_FontTexure->bindTexture(1);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
    textShader->setMat4("projection", projection);
}
