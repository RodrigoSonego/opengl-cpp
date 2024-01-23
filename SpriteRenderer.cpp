#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader spriteShader)
{
	this->spriteShader = spriteShader;
	setupRendering();
}

void SpriteRenderer::RenderSprite(Texture* tex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec2 spriteIndex, glm::vec3 color)
{
    glm::vec2* coords = SubTexture::createFromIndexes(tex, spriteIndex, size).getTexCoords();
    RenderSprite(tex, position, size, rotation, coords, color);
}

void SpriteRenderer::RenderSprite(Texture* tex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec2 texCoords[4], glm::vec3 color)
{
    spriteShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    spriteShader.setMat4("model", model);

    // render textured quad
    spriteShader.setVec3f("spriteColor", color);

    //glActiveTexture(GL_TEXTURE0);
    tex->bindTexture(0);

    //updateTextureCoordinates(newCoords);

    spriteShader.setVec2Array("texCoords", texCoords, 4);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::RenderSprite(SubTexture subTex, glm::mat4 modelMatrix, glm::vec3 color)
{
    spriteShader.use();

    spriteShader.setMat4("model", modelMatrix);

    // render textured quad
    spriteShader.setVec3f("spriteColor", color);

    //glActiveTexture(GL_TEXTURE0);
    subTex.getTexture()->bindTexture(0);

    //updateTextureCoordinates(newCoords);

    spriteShader.setVec2Array("texCoords", subTex.getTexCoords(), 4);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::RenderSprite(SubTexture subTex, glm::vec3 position, glm::vec2 size, float rotation, glm::vec3 color)
{
    RenderSprite(subTex.getTexture(), position, size, rotation, subTex.getTexCoords(), color);
}


void SpriteRenderer::setupRendering()
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

    spriteShader.use();

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::updateTextureCoordinates(glm::vec2* newTexCoords)
{
    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(float), 2 * sizeof(float), newTexCoords);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
