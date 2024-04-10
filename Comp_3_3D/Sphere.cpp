#include <iostream>
#include "NPCPath.h"
#include "Shader.h"
#include "Sphere.h"

const double M_PI = 3.141592653589793;

Sphere::Sphere(float radius, unsigned int sectCount, unsigned int stkCount, float r, float g, float b, float posX, float posY, float posZ) : Position(glm::vec3(posX, posY, posZ)), Scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
	GenerateSphere(radius, sectCount, stkCount, r, g, b);
	SetupMesh();
	rotationAngle = 0;
	rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	updateModelMatrix();
}

Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sphere::Draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", modelMatrix);
	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Sphere::UpdatePosition(const NPCPath& path, float deltaTime)
{
    pathParameter += (movingForward ? 1 : -1) * deltaTime * speed;

    if (pathParameter >= 0.2f) {
        pathParameter = 0.2f;
        movingForward = false;
    }
    else if (pathParameter <= 0.0f) {
        pathParameter = 0.0f;
        movingForward = true;
    }

    int numPoints = path.curvePoints.size();
    float t = pathParameter * (numPoints - 1);
    int index = static_cast<int>(t);
    float localT = t - index;
    int nextIndex = (movingForward ? (index + 1) % numPoints : (index > 0 ? index - 1 : numPoints - 1));

    glm::vec2 pos2D = glm::mix(path.curvePoints[index], path.curvePoints[nextIndex], localT);

    this->Position = glm::vec3(pos2D, path.position.z);
    updateModelMatrix();
}

void Sphere::GenerateSphere(float radius, unsigned int sectCount, unsigned int stkCount, float r, float g, float b)
{
    float x, y, z, xy; // vertex position
    float sectorStep = 2 * M_PI / sectCount;
    float stackStep = M_PI / stkCount;
    float sectorAngle, stackAngle;

    glm::vec3 color = { r, g, b };

    for (unsigned int i = 0; i <= stkCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)

        for (unsigned int j = 0; j <= sectCount; ++j) {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // Vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            Vertex vertex;
            vertex.Position = glm::vec3(x, y, z);
            float u = (float)j / sectCount;
            float v = (float)i / stkCount;
            vertex.TexMex = glm::vec2(u, v);

            std::cout << u << std::endl;
            // Simplistic approach for barycentric coordinates for visualization
            vertex.Barycentric = (i + j) % 2 == 0 ? glm::vec3(1.0, 0.0, 0.0) : glm::vec3(0.0, 1.0, 0.0);

            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (unsigned int i = 0; i < stkCount; ++i) {
        for (unsigned int j = 0; j < sectCount; ++j) {
            unsigned int first = i * (sectCount + 1) + j;
            unsigned int second = first + sectCount + 1;

            // Use the right hand rule to ensure the normals point outwards
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}


void Sphere::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Barycentric));
    glEnableVertexAttribArray(1);

    // Barycentric coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexMex));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, Position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, Scale);
}
