#include "Object.h"
#include "Shader.h"
#include "NPCPath.h"
#include "ObjectManager.h"
#include <iostream>

const double M_PI = 3.141592653589793;


Object::Object(int objectType, float w, float h, float d, float r,  float g, float b, float posX, float posY, float posZ) : Position(glm::vec3(posX, posY, posZ)), Scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
    if (objectType == 0) {
        GenerateCube(w, h, d, r, g, b);
        SetupArrMesh();
    }
    else if (objectType == 1) {
        GeneratePyramid((w + d) / 2, h, r, g, b);
        SetupArrMesh();
    }
    else if (objectType == 2) {
        GenerateSphere((w + h + d) / 3, 20, 20, r, g, b);
        SetupEleMesh();
    }
    else {
        std::cout << "Invalid number" << std::endl;
    }
    rotationAngle = 0;
    rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
    updateModelMatrix();
    ObjectManager::getInstance().addObject(this);
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Object::DrawArray(Shader& shader)
{
    shader.use();
    shader.setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Object::DrawElement(Shader& shader)
{
    shader.use();
    shader.setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Object::GeneratePyramid(float baseSide, float height, float r, float g, float b)
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

    vertices.push_back({ baseVertices[0], baryCoords[0], {1.0f, 0.0f} });
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

void Object::GenerateCube(float w, float h, float d, float r, float g, float b)
{
    float hw = w * 0.5f;
    float hh = h * 0.5f;
    float hd = d * 0.5f;

    glm::vec3 color = { r, g, b };

    glm::vec3 baryCoords[] = {

        {1.f, 0.f, 0.f},
        {0.f, 1.f, 0.f},
        {0.f, 0.f, 1.f}
    };

    vertices = {
    {{ hw, -hh, hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Right
    {{-hw, -hh, hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Left
    {{ hw, hh, hd}, baryCoords[2] , {1.0f, 1.0f}}, // Top Right

    {{ hw, hh, hd}, baryCoords[0] , {1.0f, 1.0f}},//Top Right
    {{-hw, hh, hd}, baryCoords[1] , {0.0f, 1.0f}},// Top Left
    {{-hw, -hh, hd}, baryCoords[2], {0.0f, 0.0f}}, //Bottom Left

    // Back face
    {{-hw, -hh, -hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw, -hh, -hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw,  hh, -hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right

    {{ hw,  hh, -hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw,  hh, -hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw, -hh, -hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left

    // Left face     
    {{-hw, -hh, -hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{-hw, -hh,  hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{-hw,  hh,  hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right

    {{-hw,  hh,  hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw,  hh, -hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw, -hh, -hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
    // Right face    

    {{ hw, -hh,  hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw, -hh, -hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw,  hh, -hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right

    {{ hw,  hh, -hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{ hw,  hh,  hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{ hw, -hh,  hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left

    // Top face      
    {{-hw,  hh,  hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw,  hh,  hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw,  hh, -hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right

    {{ hw,  hh, -hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw,  hh, -hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw,  hh,  hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left

    // Bottom face   
    {{-hw, -hh, -hd}, baryCoords[0], {1.0f, 0.0f}}, // Bottom Left
    {{ hw, -hh, -hd}, baryCoords[1], {0.0f, 0.0f}}, // Bottom Right
    {{ hw, -hh,  hd}, baryCoords[2], {1.0f, 1.0f}}, // Top Right

    {{ hw, -hh,  hd}, baryCoords[0], {1.0f, 1.0f}}, // Top Right
    {{-hw, -hh,  hd}, baryCoords[1], {0.0f, 1.0f}}, // Top Left
    {{-hw, -hh, -hd}, baryCoords[2], {0.0f, 0.0f}}, // Bottom Left
    };
}

void Object::GenerateSphere(float radius, unsigned int sectCount, unsigned int stkCount, float r, float g, float b)
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

void Object::UpdatePosition(const NPCPath& path, float deltaTime)
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

void Object::UpdateScale(int objectType, float w, float h, float d)
{
    if (objectType == 0) {
        GenerateCube(w, h, d, 0.f, 0.f, 0.f);
        SetupArrMesh();
    }
    else if (objectType == 1) {
        GeneratePyramid((w + d) / 2, h, 0.f, 0.f, 0.f);
        SetupArrMesh();
    }
    else if (objectType == 2) {
        GenerateSphere((w + h + d) / 3, 20, 20, 0.f, 0.f, 0.f);
        SetupEleMesh();
    }
    else {
        std::cout << "Invalid number" << std::endl;
    }


    updateModelMatrix();
}

Collision Object::getCollisionData() const
{
    return Collision{ Position, Scale };
}





void Object::SetupArrMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

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

void Object::SetupEleMesh()
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

void Object::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, Position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);
    modelMatrix = glm::scale(modelMatrix, Scale);
}

