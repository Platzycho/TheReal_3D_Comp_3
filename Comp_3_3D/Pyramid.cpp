#include <iostream>
#include "Shader.h"
#include "Pyramid.h"

std::vector<PyraCollision> Pyramid::pyroMids = {};

Pyramid::Pyramid(float baseSide, float height, float r, float g, float b, float posX, float posY, float posZ) : Position(glm::vec3(posX, posY, posZ)), Scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
    GeneratePyramid(baseSide, height, r, g, b);
    SetupMesh();
    rotationAngle = 0;
    rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
    updateModelMatrix();
}

Pyramid::~Pyramid()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Pyramid::Draw(Shader& shader)
{
    shader.use();

    shader.setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Pyramid::GeneratePyramid(float baseSide, float height, float r, float g, float b)
{
    float halfSide = baseSide / 2.f;

    glm::vec3 color = { r, g, b };

    glm::vec3 baryCoords[] = {
        {1.f, 0.f, 0.f},
        {0.f, 1.f, 0.f},
        {0.f, 0.f, 1.f}
    };

    glm::vec3 baseVertices[] = {
        glm::vec3(-halfSide, 0.f, halfSide), //Front-left
        glm::vec3(halfSide, 0.f, halfSide), //front-right
        glm::vec3(halfSide, 0.f, -halfSide), //back-right
        glm::vec3(-halfSide, 0.f, -halfSide)
    };

    glm::vec3 topVertex = glm::vec3(0.f, height, 0.f);

    vertices.push_back({ baseVertices[0], baryCoords[0], {1.0f, 0.0f}});
    vertices.push_back({ baseVertices[1], baryCoords[1], {0.0f, 0.0f} });
    vertices.push_back({ baseVertices[2], baryCoords[2], {1.0f, 1.0f} });

    vertices.push_back({ baseVertices[0], baryCoords[0], {1.0f, 1.0f} });
    vertices.push_back({ baseVertices[2], baryCoords[1], {0.0f, 1.0f} });
    vertices.push_back({ baseVertices[3], baryCoords[2], {0.0f, 0.0f} });

    for (int i = 0; i < 4; ++i) {
        vertices.push_back({ baseVertices[i], baryCoords[0], {1.0f, 0.0f} });
        vertices.push_back({ baseVertices[(i + 1) % 4] , baryCoords[1], {0.0f, 0.0f} });
        vertices.push_back({ topVertex, baryCoords[2], {1.0f, 1.0f} });
    }
}

void Pyramid::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PyraVertex), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PyraVertex), (void*)offsetof(PyraVertex, Position));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PyraVertex), (void*)offsetof(PyraVertex, Barycentric));
    glEnableVertexAttribArray(1);

    // Barycentric coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PyraVertex), (void*)offsetof(PyraVertex, TexMex));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Pyramid::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, Position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, Scale);
}